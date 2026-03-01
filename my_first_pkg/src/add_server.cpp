#include "example_interfaces/srv/detail/add_two_ints__struct.hpp"
#include <example_interfaces/srv/add_two_ints.hpp>
#include <memory>
#include <rclcpp/executors.hpp>
#include <rclcpp/logging.hpp>
#include <rclcpp/node.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/service.hpp>
#include <rclcpp/utilities.hpp>

class AddServer : public rclcpp::Node {
public:
  AddServer() : Node("add_server") {
    srv_ = this->create_service<example_interfaces::srv::AddTwoInts>(
        "add_two_ints",
        [this](
            const example_interfaces::srv::AddTwoInts::Request::SharedPtr req,
            example_interfaces::srv::AddTwoInts::Response::SharedPtr res) {
          res->sum = req->a + req->b;
          RCLCPP_INFO(this->get_logger(), "%ld + %ld = %ld", req->a, req->b,
                      res->sum);
        });
  }

private:
  rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr srv_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AddServer>());
  rclcpp::shutdown();
  return 0;
}
