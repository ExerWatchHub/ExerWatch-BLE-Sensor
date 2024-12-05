#include "../ExerSense/exersense.h"
#include "../ExerSense/frequency_sync.h"

namespace ExerSense
{
  class Program : public ExerProgram
  {
  private:
    ExerDisplay *display = ExerDisplay::GetInstance();
    ExerBLE *ble;

  public:
    void init()
    {
      name = "Program";
      ble = ExerBLE::GetInstance();
    }

    void cleanup() {};

    void update()
    {
      bool a = Button_A.isPressed();
      if (a)
      {
        screen_buf->setTextSize(2.2);
        screen_buf->setTextColor(TFT_YELLOW);
        screen_buf->setCursor(10, 120);
        screen_buf->printf("HEY!");
      }
      // ble->update_ble();
      screen_buf->setTextSize(1.6);
      screen_buf->setTextColor(TFT_RED);
      screen_buf->setCursor(10, 5);
      screen_buf->printf("Hello!");
      display->render();
    }
  };
}