#pragma once
#ifndef LIBS_H
#define LIBS_H

#include <Arduino.h>

// M5 Libraries
#include <M5Unified.h>
#include <M5GFX.h>
#include <M5StickCPlus2.h> // #include <M5StickC.h>
// #include <lgfx/v1/panel/Panel_ST7789.hpp>

// BLE Related
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// WiFi and web
#include <WiFi.h>
#include <esp_wifi.h>
// #include <WiFiMulti.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>

// File-system management
#include <SPIFFS.h>

// Other Libraries
#include <EasyButton.h>

#endif