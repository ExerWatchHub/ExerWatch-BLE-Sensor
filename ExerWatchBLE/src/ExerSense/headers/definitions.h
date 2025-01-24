#pragma once
#ifndef DEFS_H
#define DEFS_H

#include "types.h"

#define SOFTWARE_VERSION "0.0.1"
#define EXER_BLE_SERVICE_UUID "EC4D35AE-96DC-4385-81B2-64A17E67B13D" // Randomly generated to make sure it's unique
#define EXER_CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define EXER_CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#define WATCH_SERVICE_UUID "A12FAB51-359D-4ADF-B276-D2AD84B20134" // UART service UUID
#define WATCH_CHARACTERISTIC_UUID_RX "6E400010-B5A3-F393-E0A9-E50E24DCCA9E"
#define WATCH_CHARACTERISTIC_UUID_TX "6E400011-B5A3-F393-E0A9-E50E24DCCA9E"

#define UART_SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID

#define SCREEN_W = 240;
#define SCREEN_H = 135;
#define BG_COLOR = TFT_BLACK;

// find appropriate values for M5Stick for BUTTON A,B - see reverse of the device for numbers
#define BTN_A 37   // Main big button at the front
#define BTN_B 39   // Little button on the bottom side
#define BTN_PWR 35 // Little button on the top side (on the same side as BTN_A)
#define LONG_PRESS_DURATION 1200
// History buffer
#define CHUNK_SIZE 1
#define DUMP_WAIT_TIME -1
#define BUFFER_SIZE 1024
#define CMDS_HISTORY_SIZE 6

#define PACKET_FREQUENCY_HZ 10 // hz

namespace ExerSense
{
  EasyButton Button_A(BTN_A, 20);
  EasyButton Button_B(BTN_B, 20);
  EasyButton Button_PWR(BTN_PWR, 20);

  // #define BUFFER_SIZE 128
  exer_data_t buffer[BUFFER_SIZE]; // 40 bytes * BUFFER_SIZE = 40,960B or 40Kb if BUFFER_SIZE is 1024

  LGFX_Sprite *screen_buf;
  M5GFX display;
  String blename;
  std::string _version_ = "0.0.1";

  unsigned long start_time = -1;
  unsigned long last_update = -1;
  unsigned long total_time = 0;
  unsigned long elapsed = 0;
  unsigned long seq = 0;
  char message[256];
  char packet_str[256]; // max length

  int delta_T = 100;
  int delta_offset = 25;
  int delta_adjusted = 10;

  // Screen sizes
  int h = 0;
  int w = 0;
  float fontSize = 1.2;
  int lineHeight = 0;
  bool full = false;
  int packet_size = sizeof(buffer[0]);
  int buf_size = sizeof(buffer);
  int idx = 0;
  int dump_idx = 0;
  bool ice_breaker = true;
  bool start_read = false;
  // delta_offset is computed manually based on delta_T = 100
  // it is the time to execute the code between reading and presenting
  // data . so setup() will adjust down delta_T by delta_offset
  // 23/Feb/2021

  // for future
  float threshold = 1.2;
  int scale = 1;
  int32_t rectWidth = 60;
  bool show_cube = false;
  bool paused = false;
}
#endif