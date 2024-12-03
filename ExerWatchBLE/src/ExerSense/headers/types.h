#pragma once
#ifndef TYPES_H
#define TYPES_H

#include "libs.h"
using ImuData = m5::IMU_Class::imu_data_t;
using Point3D = m5::IMU_Class::imu_3d_t;

typedef struct
{
  double x;
  double y;
} Point2D;

typedef struct
{
  double w;
  double h;
} Size2D;

typedef struct
{
  Point3D start_point;
  Point3D end_point;
} line_3d_t;

typedef struct
{
  Point2D start;
  Point2D end;
} offset_t;

typedef struct
{
  unsigned long timestamp;  // 4 bytes
  unsigned long seq;        // 4 bytes
  unsigned long total_time; // 4 bytes
  unsigned long elapsed;    // 4 bytes
  Point3D acc;              // 4 x 3 = 12 bytes
  Point3D gyr;              // 4 x 3 = 12 bytes
} exer_data_t;              // total 4 + 4 + 4 + 4 + 12 + 12 = 40 bytes

#endif