cmake_minimum_required(VERSION 3.8)
project(my_first_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(sensor_msgs REQUIRED)
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(talker src/publisher.cpp)
ament_target_dependencies(talker rclcpp std_msgs)
install(TARGETS talker DESTINATION lib/${PROJECT_NAME})

add_executable(listener src/subscriber.cpp)
ament_target_dependencies(listener rclcpp std_msgs)
install(TARGETS listener DESTINATION lib/${PROJECT_NAME})

find_package(example_interfaces REQUIRED)
add_executable(add_server src/add_server.cpp)
ament_target_dependencies(add_server rclcpp example_interfaces)
install(TARGETS add_server DESTINATION lib/${PROJECT_NAME})

add_executable(add_client src/add_client.cpp)
ament_target_dependencies(add_client rclcpp example_interfaces)
install(TARGETS add_client DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

find_package(my_interfaces REQUIRED)
add_executable(sensor_pub src/sensor_pub.cpp)
ament_target_dependencies(sensor_pub rclcpp my_interfaces)
install(TARGETS sensor_pub DESTINATION lib/${PROJECT_NAME})

add_executable(ekf_slam src/ekf_slam.cpp)
ament_target_dependencies(ekf_slam rclcpp geometry_msgs visualization_msgs Eigen3)
target_link_libraries(ekf_slam Eigen3::Eigen)
install(TARGETS ekf_slam DESTINATION lib/${PROJECT_NAME})

ament_package()
