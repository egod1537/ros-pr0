#include <bits/stdc++.h>
#include <chrono>
#include <example_interfaces/srv/add_two_ints.hpp>
#include <example_interfaces/srv/detail/add_two_ints__struct.hpp>
#include <memory>
#include <rclcpp/executors.hpp>
#include <rclcpp/future_return_code.hpp>
#include <rclcpp/logging.hpp>
#include <rclcpp/node.hpp>
#include <rclcpp/utilities.hpp>

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("add_client");
  auto client =
      node->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints");

  while (!client->wait_for_service(std::chrono::seconds(1))) {
    RCLCPP_INFO(node->get_logger(), "Waiting for server...");
  }

  auto req = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
  req->a = 10;
  req->b = 32;

  auto future = client->async_send_request(req);

  if (rclcpp::spin_until_future_complete(node, future) ==
      rclcpp::FutureReturnCode::SUCCESS) {
    RCLCPP_INFO(node->get_logger(), "Result: %ld", future.get()->sum);
  }

  rclcpp::shutdown();
  return 0;
}
