#include <cmath>
#include <cstdlib>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/src/Core/Matrix.h>
#include <geometry_msgs/msg/twist.hpp>
#include <memory>
#include <rcl/node.h>
#include <rclcpp/executors.hpp>
#include <rclcpp/logging.hpp>
#include <rclcpp/node.hpp>
#include <rclcpp/publisher.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/subscription.hpp>
#include <rclcpp/utilities.hpp>
#include <sensor_msgs/msg/detail/laser_scan__struct.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <vector>
#include <visualization_msgs/msg/detail/marker__struct.hpp>
#include <visualization_msgs/msg/detail/marker_array__struct.hpp>
#include <visualization_msgs/msg/marker_array.hpp>

using Twist = geometry_msgs::msg::Twist;
using LaserScan = sensor_msgs::msg::LaserScan;
using MarkerArray = visualization_msgs::msg::MarkerArray;
using Marker = visualization_msgs::msg::Marker;
using VecX = Eigen::VectorXd;
using MatX = Eigen::MatrixXd;
using Vec2 = Eigen::Vector2d;

struct Observation {
  double r;
  double phi;
};

static std::vector<Observation> extract_landmarks(const LaserScan &scan,
                                                  double cluster_thresh = 0.3,
                                                  int min_pts = 3,
                                                  double max_range = 3.5) {
  std::vector<Observation> obs;
  std::vector<Vec2> cluster;

  auto flush = [&]() {
    if ((int)cluster.size() < min_pts) {
      cluster.clear();
      return;
    }

    Vec2 c = Vec2::Zero();
    for (auto &p : cluster)
      c += p;
    c /= cluster.size();

    double r = c.norm();
    double phi = std::atan2(c.y(), c.x());
    obs.push_back({r, phi});
    cluster.clear();
  };

  for (size_t i = 0; i < scan.ranges.size(); ++i) {
    float ri = scan.ranges[i];

    if (ri < scan.range_min || ri > scan.range_max) {
      flush();
      continue;
    }

    double ang = scan.angle_min + i * scan.angle_increment;
    Vec2 pt(ri * std::cos(ang), ri * std::sin(ang));

    if (!cluster.empty() && (pt - cluster.back()).norm() > cluster_thresh) {
      flush();
    }
    cluster.push_back(pt);
  }
  flush();

  return obs;
}

class EkfsSlam : public rclcpp::Node {
public:
  EkfsSlam() : Node("ekf_slam"), n_lm_(0) {
    x_ = VecX::Zero(3);
    S_ = MatX::Identity(3, 3) * 0.01;

    R_ = Eigen::Matrix2d::Zero();
    R_(0, 0) = 0.1;
    R_(1, 1) = 0.05;

    sub_cmd_ = create_subscription<Twist>("cmd_vel", 10,
                                          [this](const Twist::SharedPtr m) {
                                            predict(m->linear.x, m->angular.z);
                                          });

    sub_scan_ = create_subscription<LaserScan>(
        "scan", 10, [this](const LaserScan::SharedPtr m) {
          auto obs = extract_landmarks(*m);
          for (auto &z : obs)
            update(z);
          log_state();
        });

    pub_markers_ = create_publisher<MarkerArray>("ekf_landmarks", 10);
  }

private:
  int n_lm_;
  VecX x_;
  MatX S_;
  Eigen::Matrix2d R_;

  rclcpp::Subscription<Twist>::SharedPtr sub_cmd_;
  rclcpp::Subscription<LaserScan>::SharedPtr sub_scan_;
  rclcpp::Publisher<MarkerArray>::SharedPtr pub_markers_;

  static double wrap(double a) {
    while (a > M_PI)
      a -= 2 * M_PI;
    while (a < -M_PI)
      a += 2 * M_PI;
    return a;
  }

  int state_dim() const { return 3 + 2 * n_lm_; }

  void predict(double v, double w) {
    if (std::abs(v) < 1e-6 && std::abs(w) < 1e-6)
      return;

    double dt = 0.1;
    double th = x_(2);

    x_(0) += v * dt * std::cos(th);
    x_(1) += v * dt * std::sin(th);
    x_(2) += w * dt;
    x_(2) = wrap(x_(2));

    int n = state_dim();
    MatX F = MatX::Identity(n, n);
    F(0, 2) = -v * dt * std::sin(th);
    F(1, 2) = v * dt * std::cos(th);

    MatX Q = MatX::Zero(n, n);
    Q(0, 0) = 0.1 * std::abs(v);
    Q(1, 1) = 0.1 * std::abs(v);
    Q(2, 2) = 0.05 * std::abs(w);

    S_ = F * S_ * F.transpose() + Q;
  }

  void update(const Observation &z) {
    int j = associate(z);
    if (j < 0) {
      init_landmark(z);
      return;
    }

    int li = 3 + 2 * j;
    int n = state_dim();

    double dx = x_(li) - x_(0);
    double dy = x_(li + 1) - x_(1);
    double q = dx * dx + dy * dy;
    double sq = std::sqrt(q);

    Eigen::Vector2d z_hat;
    z_hat(0) = sq;
    z_hat(1) = wrap(std::atan2(dy, dx) - x_(2));

    MatX H = MatX::Zero(2, n);

    H(0, 0) = -dx / q;
    H(0, 1) = -dy / q;
    H(0, 2) = 0;
    H(1, 0) = dy / sq;
    H(1, 1) = -dx / sq;
    H(0, 2) = -1;

    H(0, li) = dx / sq;
    H(0, li + 1) = dy / sq;
    H(1, li) = -dy / q;
    H(1, li) = dx / q;

    Eigen::Matrix2d Z = H * S_ * H.transpose() + R_;
    MatX K = S_ * H.transpose() * Z.inverse();

    Eigen::Vector2d innov;
    innov(0) = z.r - z_hat(0);
    innov(1) = wrap(z.phi - z_hat(1));

    x_ += K * innov;
    x_(2) = wrap(x_(2));

    S_ = (MatX::Identity() - K * H) * S_;
  }

  int associate(const Observation &z) {
    double best_d = 1.5;
    int best_j = -1;

    for (int j = 0; j < n_lm_; ++j) {
      int li = 3 + 2 * j;

      double dx = x_(li) - x_(0);
      double dy = x_(li + 1) - x_(1);
      double q = dx * dx + dy * dy;
      double sq = std::sqrt(q);

      Eigen::Vector2d z_hat(sq, wrap(std::atan2(dy, dx) - x_(2)));

      int n = state_dim();
      MatX H = MatX::Zero(2, n);
      H(0, 0) = -dx / sq;
      H(0, 1) = -dy / sq; // 로봇 부분
      H(1, 0) = dy / q;
      H(1, 1) = -dx / q;
      H(1, 2) = -1;
      H(0, li) = dx / sq;
      H(0, li + 1) = dy / sq; // 랜드마크 부분
      H(1, li) = -dy / q;
      H(1, li + 1) = dx / q;

      Eigen::Matrix2d Z = H * S_ * H.transpose() + R_;
      Eigen::Vector2d innov(z.r - z_hat(0), wrap(z.phi - z_hat(1)));

      double d = std::sqrt(innov.transpose() * Z.inverse() * innov);
      if (d < best_d) {
        best_d = d;
        best_j = j;
      }
    }

    return best_j;
  }

  void init_landmark(const Observation &z) {
    double th = x_(2);

    double lx = x_(0) + z.r * std::cos(th + z.phi);
    double ly = x_(1) + z.r * std::sin(th + z.phi);

    int n_old = state_dim();
    n_lm_++;
    int n_new = state_dim();

    VecX x_new = VecX::Zero(n_new);
    x_new.head(n_old) = x_;
    x_new(n_old) = lx;
    x_new(n_old + 1) = ly;
    x_ = x_new;

    MatX S_new = MatX::Zero(n_new, n_new);
    S_new.topLeftCorner(n_old, n_old) = S_;
    S_new(n_old, n_old) = 1.0;
    S_new(n_old + 1, n_old + 1) = 1.0;
    S_ = S_new;

    RCLCPP_INFO(get_logger(), "NEW LM #%d (%.2f, %.2f)", n_lm_, lx, ly);
  }

  void log_state() {
    RCLCPP_INFO(get_logger(),
                "pose=(%.2f, %.2f, %.1f) landmarks=%d o=(%.3f, %.3f, %.3f)",
                x_(0), x_(1), x_(2) * 180 / M_PI, n_lm_, S_(0, 0), S_(1, 1),
                S_(2, 2));

    MarkerArray ma;
    for (int j = 0; j < n_lm_; ++j) {
      Marker m;
      m.header.frame_id = "odom";
      m.header.stamp = now();
      m.ns = "landmarks";
      m.id = j;
      m.type = Marker::CYLINDER;
      m.action = Marker::ADD;

      m.pose.position.x = x_(3 + 2 * j);
      m.pose.position.y = x_(3 + 2 * j + 1);
      m.pose.position.z = 0.15;

      m.scale.x = m.scale.y = 0.15;
      m.scale.z = 0.3;

      m.color.r = 0.0;
      m.color.g = 1.0;
      m.color.b = 0.0;
      m.color.a = 0.9;

      ma.markers.push_back(m);
    }

    pub_markers_->publish(ma);
  }
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<EkfsSlam>());
  rclcpp::shutdown();
  return 0;
}
