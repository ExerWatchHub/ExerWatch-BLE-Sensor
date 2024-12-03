/**
 * Gabryxx7
 * Commands:
 * - Single Button A Press: Pause (Elapsed time keeps increasing as that is the time since restart of the device) but seq and elapsed time will pause
 * - Button A hold for 1s: Restart the sensor
 */

#include "exersense.h"

ExerSense::ExerSense *app;

namespace ExerSense
{
  void toggle_pause()
  {
    paused = !paused;
    if (!paused)
    {
      last_update = millis();
    }
  }

  void reset()
  {
    screen_buf->setTextColor(TFT_YELLOW);
    screen_buf->setCursor(10, h * 0.5);
    screen_buf->setTextSize(1.8);
    screen_buf->printf("RESTARTING...");
    delay(20);
    app->display->render();
    delay(800);
    ESP.restart();
  }
}