// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from my_interfaces:msg/Sensor.idl
// generated code does not contain a copyright notice

#ifndef MY_INTERFACES__MSG__DETAIL__SENSOR__STRUCT_HPP_
#define MY_INTERFACES__MSG__DETAIL__SENSOR__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__my_interfaces__msg__Sensor __attribute__((deprecated))
#else
# define DEPRECATED__my_interfaces__msg__Sensor __declspec(deprecated)
#endif

namespace my_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Sensor_
{
  using Type = Sensor_<ContainerAllocator>;

  explicit Sensor_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0l;
      this->tempature = 0.0;
      this->humidity = 0.0;
      this->location = "";
    }
  }

  explicit Sensor_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : location(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0l;
      this->tempature = 0.0;
      this->humidity = 0.0;
      this->location = "";
    }
  }

  // field types and members
  using _id_type =
    int32_t;
  _id_type id;
  using _tempature_type =
    double;
  _tempature_type tempature;
  using _humidity_type =
    double;
  _humidity_type humidity;
  using _location_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _location_type location;

  // setters for named parameter idiom
  Type & set__id(
    const int32_t & _arg)
  {
    this->id = _arg;
    return *this;
  }
  Type & set__tempature(
    const double & _arg)
  {
    this->tempature = _arg;
    return *this;
  }
  Type & set__humidity(
    const double & _arg)
  {
    this->humidity = _arg;
    return *this;
  }
  Type & set__location(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->location = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    my_interfaces::msg::Sensor_<ContainerAllocator> *;
  using ConstRawPtr =
    const my_interfaces::msg::Sensor_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<my_interfaces::msg::Sensor_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<my_interfaces::msg::Sensor_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      my_interfaces::msg::Sensor_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<my_interfaces::msg::Sensor_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      my_interfaces::msg::Sensor_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<my_interfaces::msg::Sensor_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<my_interfaces::msg::Sensor_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<my_interfaces::msg::Sensor_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__my_interfaces__msg__Sensor
    std::shared_ptr<my_interfaces::msg::Sensor_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__my_interfaces__msg__Sensor
    std::shared_ptr<my_interfaces::msg::Sensor_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Sensor_ & other) const
  {
    if (this->id != other.id) {
      return false;
    }
    if (this->tempature != other.tempature) {
      return false;
    }
    if (this->humidity != other.humidity) {
      return false;
    }
    if (this->location != other.location) {
      return false;
    }
    return true;
  }
  bool operator!=(const Sensor_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Sensor_

// alias to use template instance with default allocator
using Sensor =
  my_interfaces::msg::Sensor_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace my_interfaces

#endif  // MY_INTERFACES__MSG__DETAIL__SENSOR__STRUCT_HPP_
