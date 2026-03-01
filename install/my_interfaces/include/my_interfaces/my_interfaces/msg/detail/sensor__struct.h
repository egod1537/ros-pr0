// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from my_interfaces:msg/Sensor.idl
// generated code does not contain a copyright notice

#ifndef MY_INTERFACES__MSG__DETAIL__SENSOR__STRUCT_H_
#define MY_INTERFACES__MSG__DETAIL__SENSOR__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'location'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/Sensor in the package my_interfaces.
typedef struct my_interfaces__msg__Sensor
{
  int32_t id;
  double tempature;
  double humidity;
  rosidl_runtime_c__String location;
} my_interfaces__msg__Sensor;

// Struct for a sequence of my_interfaces__msg__Sensor.
typedef struct my_interfaces__msg__Sensor__Sequence
{
  my_interfaces__msg__Sensor * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} my_interfaces__msg__Sensor__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MY_INTERFACES__MSG__DETAIL__SENSOR__STRUCT_H_
