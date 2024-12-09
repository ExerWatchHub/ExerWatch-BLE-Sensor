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

  void send_inertial(ImuData &data, float millis)
  {
    // Inertial Message Format: "I,timeStamp (µs),gx,gy,gz,ax,ay,az\r\n"
    char* msg;
    sprintf(msg, "I,%d,%5.4f,%5.4f,%5.4f,%5.4f,%5.4f,%5.4f\r\n", millis * 1000, data.gyro.x, data.gyro.y, data.gyro.z, data.accel.x, data.accel.y, data.accel.z);
    Serial.print(msg);
  }

  // void Reefwing_xIMU3::sendInertial(InertialMessage msg)
  // {
  //   // Inertial Message Format: "I,timeStamp (µs),gx,gy,gz,ax,ay,az\r\n"
  //   Serial.print("I,");
  //   Serial.print(msg.timeStamp);
  //   Serial.print(",");
  //   Serial.print(msg.gx, 4);
  //   Serial.print(",");
  //   Serial.print(msg.gy, 4);
  //   Serial.print(",");
  //   Serial.print(msg.gz, 4);
  //   Serial.print(",");
  //   Serial.print(msg.ax, 4);
  //   Serial.print(",");
  //   Serial.print(msg.ay, 4);
  //   Serial.print(",");
  //   Serial.print(msg.az, 4);
  //   Serial.print("\r\n");
  // }
}
#endif