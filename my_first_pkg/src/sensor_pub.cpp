#include <chrono>
#include <memory>
#include <my_interfaces/msg/sensor.hpp>
#include <rclcpp/executors.hpp>
#include <rclcpp/logging.hpp>
#include <rclcpp/node.hpp>
#include <rclcpp/publisher_base.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/timer.hpp>
#include <rclcpp/utilities.hpp>

using Sensor = my_interfaces::msg::Sensor;

class SensorPub : public rclcpp::Node {
public:
  SensorPub() : Node("sensor_pub") {
    pub_ = this->create_publisher<Sensor>("sensor_data", 10);
    timer_ = this->create_wall_timer(std::chrono::seconds(1), [this]() {
      auto msg = Sensor();
      msg.id = count_++;
      msg.tempature = 23.5 + (count_ % 10) * 0.1;
      msg.humidity = 45.0;
      msg.location = "room_a";

      RCLCPP_INFO(this->get_logger(), "id=%d temp=%.1f", msg.id, msg.tempature);
      pub_->publish(msg);
    });
  }

private:
  rclcpp::Publisher<Sensor>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  int count_ = 0;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SensorPub>());
  rclcpp::shutdown();
  return 0;
}
