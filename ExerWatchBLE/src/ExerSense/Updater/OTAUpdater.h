
#ifndef OTA_H
#define OTA_H

#include "../headers/commons.h"

// WiFi Credentials
#ifndef APSSID
#define APSSID "<YOUR-SSID>"
#define APPSK "<YOUR-PASSWD>"
#endif

namespace OTAUpdater
{

  struct WiFiList_t
  {
    String ssid;
    int8_t rssi = 0;
  };
  static WiFiList_t _wifi_list[13];
  static bool _wifi_tested = false;

  class OTAUpdater : public ExerProgram
  {
  private:
    String app_version = "";

  public:
    OTAUpdater()
    {
      get_app_version();
    }

    String get_app_version()
    {
      File version_file = SPIFFS.open(F("/version.txt"), "r");
      if (version_file)
      {
        app_version = version_file.readString();
        version_file.close();
      }
      return app_version;
    }

    void init()
    {
      name = "OTAUpdater";
      // Set WiFi to station mode and disconnect from an AP if it was previously connected.
      WiFi.mode(WIFI_STA);
      WiFi.disconnect();
    }

    void loop()
    {
    }
  };
}

#endif