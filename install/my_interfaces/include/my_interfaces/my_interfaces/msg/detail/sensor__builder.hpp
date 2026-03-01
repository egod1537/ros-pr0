// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from my_interfaces:msg/Sensor.idl
// generated code does not contain a copyright notice

#ifndef MY_INTERFACES__MSG__DETAIL__SENSOR__BUILDER_HPP_
#define MY_INTERFACES__MSG__DETAIL__SENSOR__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "my_interfaces/msg/detail/sensor__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace my_interfaces
{

namespace msg
{

namespace builder
{

class Init_Sensor_location
{
public:
  explicit Init_Sensor_location(::my_interfaces::msg::Sensor & msg)
  : msg_(msg)
  {}
  ::my_interfaces::msg::Sensor location(::my_interfaces::msg::Sensor::_location_type arg)
  {
    msg_.location = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_interfaces::msg::Sensor msg_;
};

class Init_Sensor_humidity
{
public:
  explicit Init_Sensor_humidity(::my_interfaces::msg::Sensor & msg)
  : msg_(msg)
  {}
  Init_Sensor_location humidity(::my_interfaces::msg::Sensor::_humidity_type arg)
  {
    msg_.humidity = std::move(arg);
    return Init_Sensor_location(msg_);
  }

private:
  ::my_interfaces::msg::Sensor msg_;
};

class Init_Sensor_tempature
{
public:
  explicit Init_Sensor_tempature(::my_interfaces::msg::Sensor & msg)
  : msg_(msg)
  {}
  Init_Sensor_humidity tempature(::my_interfaces::msg::Sensor::_tempature_type arg)
  {
    msg_.tempature = std::move(arg);
    return Init_Sensor_humidity(msg_);
  }

private:
  ::my_interfaces::msg::Sensor msg_;
};

class Init_Sensor_id
{
public:
  Init_Sensor_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Sensor_tempature id(::my_interfaces::msg::Sensor::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_Sensor_tempature(msg_);
  }

private:
  ::my_interfaces::msg::Sensor msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_interfaces::msg::Sensor>()
{
  return my_interfaces::msg::builder::Init_Sensor_id();
}

}  // namespace my_interfaces

#endif  // MY_INTERFACES__MSG__DETAIL__SENSOR__BUILDER_HPP_
