#pragma once
#ifndef EXERSENSE_H
#define EXERSENSE_H

#include "headers/commons.h"
#include "ble/ble.h"
#include "display.h"
#include "sensors.h"
#include "imu_3d.h"

namespace ExerSense
{
  void reset();        // Forward declaration
  void toggle_pause(); // Forward declaration
  class ExerSense : public ExerProgram
  {
  protected:
    ExerSense(){};
    static ExerSense *singleton_;

  public:
    static ExerSense *GetInstance();
    ExerBLE *ble;
    ExerDisplay *display;

    // private:
    //   void setup();
    //   void loop();
    //   void restart_device();
    //   // void reset();
    //   // void toggle_pause();
    //   void update();
    //   void print_time(Point2D pos);

    void restart_device()
    {
      Serial.println("Restarting device...");
      ESP.restart();
    }

    void init()
    {
      if (is_init)
        return;
      name = "ExerSense";
      is_init = true;
      Serial.begin(115200);
      auto cfg = M5.config();
      StickCP2.begin(cfg); // This should automatically init all hardware

      ble = ExerBLE::GetInstance();
      // ble->setPacketFrequency();

      ble->init_ble();
      display->init_display();

      Button_A.begin();
      Button_B.begin();
      Button_PWR.begin();

      // Button_B.onPressed(on_B_Pressed);
      Button_A.onPressed(toggle_pause);
      Button_A.onPressedFor(LONG_PRESS_DURATION, reset);
    }

    void update_status()
    {
      if (start_time < 0)
      {
        display->print_metadata();
        start_time = millis();
      }
      if (!paused)
      {
        seq++;
        elapsed += millis() - last_update;
      }
      total_time = millis() - start_time;
      last_update = millis();
    }

    void print_time(Point2D pos)
    {
      screen_buf->setTextColor(paused? TFT_DARKGRAY : TFT_LIGHTGRAY);
      if (paused)
      {
        screen_buf->setCursor(120, 120);
        screen_buf->printf("(PAUSED)");
      }
      screen_buf->setCursor(pos.x, pos.y);
      screen_buf->printf("Time: %6.2f / %6.2f", elapsed / 1000.0, total_time / 1000.0);
      screen_buf->setCursor(pos.x, pos.y + lineHeight);
      screen_buf->printf("Sent/Seq: %d/%d", ble->totalSentPackets, seq);
    }

    void update()
    {
      auto imu_data = StickCP2.Imu.getImuData();

      screen_buf->clear();
      StickCP2.Imu.update();
      if (show_cube)
      {
        IMU3DViz::draw_3d_cube();
      }
      print_imu_data({.x = 5, .y = 90}, imu_data, elapsed, seq);
      print_time({.x = 5, .y = 65});

      ble->update_ble();
      ble->print_ble_data({.x = 5, .y = 10});

      update_status();
      display->render();
      delay(20);
    }
  };
}

ExerSense::ExerSense *ExerSense::ExerSense::singleton_ = nullptr;
ExerSense::ExerSense *ExerSense::ExerSense::GetInstance()
{
  if (singleton_ == nullptr)
  {
    singleton_ = new ExerSense();
  }
  return singleton_;
}
#endif