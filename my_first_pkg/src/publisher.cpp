#include "rclcpp/logging.hpp"
#include "rclcpp/utilities.hpp"
#include <chrono>
#include <cstdio>
#include <cwchar>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/timer.hpp>
#include <std_msgs/msg/string.hpp>

class MinimalPublisher : public rclcpp::Node {
public:
  MinimalPublisher() : Node("Minimal_publisher"), count_(0) {
    pub_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer_ = this->create_wall_timer(std::chrono::milliseconds(500), [this]() {
      auto msg = std_msgs::msg::String();
      msg.data = "Hello: " + std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(), "Publishing: %s", msg.data.c_str());
      pub_->publish(msg);
    });
  }

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
