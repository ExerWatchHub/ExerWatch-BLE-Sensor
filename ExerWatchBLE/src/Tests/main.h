#include "../ExerSense/exersense.h"
#include "../ExerSense/frequency_sync.h"

namespace ExerSense
{
  class Tests : public ExerProgram
  {
  private:
    ExerDisplay *display = ExerDisplay::GetInstance();
    FrequencySync *freqSync = new FrequencySync(10);
    ExerBLE *ble;

  public:
    void init()
    {
      name = "Tests";
      ble = ExerBLE::GetInstance();
    }

    void cleanup() {};

    void update()
    {
      bool a = Button_A.isPressed();
      if (a)
      {
        delay(10); // For testing purposes
      }
      else
      {
        freqSync->tick();
        freqSync->syncDelay();
      }
      // ble->update_ble();
      screen_buf->setTextSize(1.6);
      screen_buf->setTextColor(TFT_RED);
      screen_buf->setCursor(10, 5);
      screen_buf->printf("Sent %d pkts in %dms", freqSync->lastSentPackets, freqSync->lastElapsed);
      screen_buf->setCursor(10, 20);
      screen_buf->printf("Freq: %2.2fhz", freqSync->currentFrequency);
      if (a)
      {
        screen_buf->setTextSize(2.2);
        screen_buf->setTextColor(TFT_YELLOW);
        screen_buf->setCursor(10, 120);
        screen_buf->printf("PAUSE");
      }
      display->render();
    }
  };
}