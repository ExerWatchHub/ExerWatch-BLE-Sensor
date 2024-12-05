#include <Arduino.h>
#include <vector>
#include "ExerSense/main.h"
#include "Tests/main.h"
#include "Program/main.h"
// #include "ExerSense/wifi/wifiscanner.h"

/**
 * Gabryxx7
 * Commands:
 * - Single Button A Press: Pause (Elapsed time keeps increasing as that is the time since restart of the device) but seq and elapsed time will pause
 * - Button A hold for 1s: Restart the sensor
 * As explained here https://forum.arduino.cc/t/esp32-assert-failed-vqueuedelete-queue-c-2131-pxqueue/1122763/3:
 * "Oh yea, the ESP32 only has a single radio transceiver. It can be used for WiFi or Bluetooth but not both simultaneously. So I can definitely see how trying to do so will cause problems unless you're very careful about turning off / disabling one mode before trying to use the other. And even then, the new mode may require some type of re-initialization. I'm not sue, never tried it."
 */

using namespace std;
#define STARTING_PROGRAM 0
ExerSense::Tests *test;
ExerSense::Program *program;
// Networking::WiFiScanner *wifi_test;

vector<ExerProgram *> programs;
ExerProgram* current_program;
int p_idx = STARTING_PROGRAM;
long prev_time = millis();
// Testing cycling views for future debug purposes
void next_program()
{
  p_idx = p_idx >= programs.size()-1 ? 0 : p_idx + 1;
}
void prev_program()
{
  p_idx = p_idx <= 0 ? programs.size() -1 : p_idx - 1;
}

void init_programs()
{
  // App should always be initialized first as it inits the hardware
  // Might have to change this program to use a generic "ProgramManager" class that does that independently
  app = ExerSense::ExerSense::GetInstance();
  app->init();
  programs.push_back(app);
  test = new ExerSense::Tests();
  programs.push_back(test);

  program = new ExerSense::Program();
  programs.push_back(program);
  // wifi_test = Networking::WiFiScanner::GetInstance();
  // programs.push_back(wifi_test);
  // Always skip i = 0 as that's the main app and it's initialized separately
  for(int i = 1; i < programs.size(); i++){
    programs[i]->init();
  }
}

void print_program_debug()
{
  auto screen_buf = ExerSense::screen_buf;
  float elapsed = (float) (millis() - prev_time) / 1000;
  screen_buf->clear();
  screen_buf->fillScreen(TFT_BLACK);
  screen_buf->setTextColor(TFT_RED);
  screen_buf->setCursor(2, 50);
  screen_buf->setTextSize(1.4);
  screen_buf->printf("%3.2f  %d/%d  %s", elapsed, p_idx+1, programs.size(), programs[p_idx]->getName());
  screen_buf->pushSprite(0,0);
}

void setup()
{
  init_programs();
  ExerSense::Button_PWR.onPressed(prev_program);
  ExerSense::Button_B.onPressed(next_program);
}

void loop()
{
  ExerSense::Button_A.read();
  ExerSense::Button_B.read();
  ExerSense::Button_PWR.read();
  ExerProgram* p = programs[p_idx];
  // print_program_debug();
  p->update();
  delay(20);
}
