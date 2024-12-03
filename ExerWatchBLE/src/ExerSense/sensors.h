#pragma once
#ifndef SENSORS_H
#define SENSORS_H

#include "headers/commons.h"

namespace ExerSense{

  char serial_msg[256];

  void print_acc(Point3D acc, Point2D pos)
  {
    sprintf(serial_msg, ", %+6.2f, %+6.2f, %+6.2f", acc.x, acc.y, acc.z);
    screen_buf->setTextColor(TFT_DARKGREEN);
    screen_buf->setCursor(pos.x, pos.y);
    pos.y += lineHeight;
    screen_buf->printf("Acc XYZ:");
    screen_buf->setCursor(pos.x, pos.y);
    pos.y += lineHeight;
    screen_buf->printf("%+6.2f  ", acc.x);
    screen_buf->setCursor(pos.x, pos.y);
    pos.y += lineHeight;
    screen_buf->printf("%+6.2f  ", acc.y);
    screen_buf->setCursor(pos.x, pos.y);
    pos.y += lineHeight;
    screen_buf->printf("%+6.2f  ", acc.z);
  }

  void print_gyr(Point3D gyr, Point2D pos)
  {
    sprintf(serial_msg, ", %+6.2f, %+6.2f, %+6.2f", gyr.x, gyr.y, gyr.z);
    screen_buf->setTextColor(TFT_DARKCYAN);
    screen_buf->setCursor(pos.x, pos.y);
    pos.y += lineHeight;
    screen_buf->printf("Gyr XYZ:");
    screen_buf->setCursor(pos.x, pos.y);
    pos.y += lineHeight;
    screen_buf->printf("%+6.2f", gyr.x);
    screen_buf->setCursor(pos.x, pos.y);
    pos.y += lineHeight;
    screen_buf->printf("%+6.2f", gyr.y);
    screen_buf->setCursor(pos.x, pos.y);
    pos.y += lineHeight;
    screen_buf->printf("%+6.2f", gyr.z);
  }

  /// SCreen routines
  void print_imu_data(Point2D pos, ImuData &data, float secs, int seq)
  {
    serial_msg[0] = 0; // "Empty" the string
    sprintf(serial_msg, "%6.3f, %04d", secs, (int)seq);
    print_acc(data.accel, {.x = pos.x, .y = pos.y});
    print_gyr(data.gyro, {.x = pos.x + 60, .y = pos.y});
    // Serial.print(message);
  }
}
#endif