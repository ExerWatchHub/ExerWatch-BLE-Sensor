
#pragma once
#ifndef WIFI_H
#define WIFI_H

#include "../headers/commons.h"
#include "../display.h"

#define WIFI_LIST_LENGTH 13

// using namespace ExerSense;

namespace Networking
{
  struct WiFiList_t
  {
    String ssid;
    int8_t rssi = 0;
  };

  static WiFiList_t _wifi_list[WIFI_LIST_LENGTH];

  String encryption_string(wifi_auth_mode_t auth_mode)
  {
    switch (auth_mode)
    {
    case WIFI_AUTH_OPEN:
      return "open";
    case WIFI_AUTH_WEP:
      return "WEP";
    case WIFI_AUTH_WPA_PSK:
      return "WPA";
    case WIFI_AUTH_WPA2_PSK:
      return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK:
      return "WPA+WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE:
      return "WPA2-EAP";
    case WIFI_AUTH_WPA3_PSK:
      return "WPA3";
    case WIFI_AUTH_WPA2_WPA3_PSK:
      return "WPA2+WPA3";
    case WIFI_AUTH_WAPI_PSK:
      return "WAPI";
    default:
      return "none";
    }
  }

  class WiFiScanner : public ExerProgram
  {

  protected:
    WiFiScanner(){};
    static WiFiScanner *singleton_;

  public:
    static WiFiScanner *GetInstance();

  private:
    void scan_networks()
    {
      int n = WiFi.scanNetworks();
      if (n <= 0)
        return;
      for (int i = 0; i < WIFI_LIST_LENGTH; ++i)
      {
        // display->setTextColor((WiFi.RSSI(i) > -75) ? TFT_GREEN : TFT_RED, TFT_BLACK);
        // display->printf(" %s ", WiFi.SSID(i).c_str());
        // display->printf(" %d\n", WiFi.RSSI(i));
        // displayUpdate();
        _wifi_list[i].rssi = WiFi.RSSI(i);
        _wifi_list[i].ssid = WiFi.SSID(i);
        String enc_str = encryption_string(WiFi.encryptionType(i));
        _wifi_list[i].ssid += "(" + enc_str + ")";
      }
      // Delete the scan result to free memory for code below.
      WiFi.scanDelete();
    }

    void print_networks_list()
    {
      auto display = ExerSense::screen_buf;
      display->clear();
      /* Display result */
      for (int i = 0; i < WIFI_LIST_LENGTH; ++i)
      {
        display->setTextColor((_wifi_list[i].rssi > -75) ? TFT_GREEN : TFT_RED, TFT_BLACK);
        display->printf(" %s ", _wifi_list[i].ssid.c_str());
        display->printf(" %d\n", _wifi_list[i].rssi);

        // _wifi_list[i].rssi = WiFi.RSSI(i);
        // _wifi_list[i].ssid = WiFi.SSID(i);
      }
    }

  public:
    void init()
    {
      if (is_init)
        return;
      is_init = true;
      name = "WiFiScanner";
      // Set WiFi to station mode and disconnect from an AP if it was previously connected.
      WiFi.mode(WIFI_STA);
      // WiFi.disconnect();
    }

    void update()
    {
      // scan_networks();
      // print_networks_list();
    }

    void cleanup()
    {
      WiFi.disconnect(true);
      esp_wifi_disconnect();
      esp_wifi_stop();
      esp_wifi_deinit();
      esp_wifi_clear_ap_list();
    }
  };
}

Networking::WiFiScanner *Networking::WiFiScanner::singleton_ = nullptr;
Networking::WiFiScanner *Networking::WiFiScanner::GetInstance()
{
  if (singleton_ == nullptr)
  {
    singleton_ = new WiFiScanner();
  }
  return singleton_;
}

#endif