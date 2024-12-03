#pragma once
#ifndef UI_H
#define UI_H

namespace ExerSense
{
  class ExerDisplay{
    protected:
      ExerDisplay(){};
      static ExerDisplay *singleton_;

    public:      
      static ExerDisplay* GetInstance();

    public: void print_metadata()
    {
      // sprintf(message,"{\"Device\":\"%s\",\"period\":%d}\n",blename.c_str(),delta_T);
      sprintf(message, "{\"Version\":\"%s\",\"Device\":\"%s\",\"period(ms)\":%d}\n", _version_.c_str(), blename.c_str(), delta_T);
      sprintf(message, "Secs,SeqNo,AccX,AccY,AccZ,GyroX,GyroY,GyroZ\n");
      // Serial.print(message);
    }

  public: void init_display()
    {
      /* Do not print to the display directly
        Instead use the screen_buf buffer, which will be rendered on scereen once at the end!
        */
      display = StickCP2.Lcd;
      display.setRotation(3);
      display.setTextColor(WHITE);
      display.setTextDatum(middle_center);
      display.setFont(&fonts::FreeMono9pt7b);
      display.setTextSize(1);
      display.clear(BLACK);
      display.fillScreen(BLACK);

      /* Full scren sprite used as buffer
        Reduces flickering and improves performance by only printing on screen once at the end
      */
      screen_buf = new LGFX_Sprite(&display);
      screen_buf->createSprite(display.width(), display.height());
      screen_buf->fillScreen(TFT_BLACK);
      screen_buf->setTextSize(fontSize);
      lineHeight = fontSize * 8 + 2; // The height of a character is roughly fontSize*4, added some spacing (2) around it
      w = display.width();
      h = display.height();
    }

  private: void reset_buffer()
  {
    screen_buf->fillScreen(TFT_BLACK);
    screen_buf->setTextSize(fontSize);
    screen_buf->setTextColor(WHITE);
    screen_buf->setCursor(0,0);
  }

  public: void render()
    {
      screen_buf->pushSprite(0, 0);
      reset_buffer();
    }
  };
}

ExerSense::ExerDisplay *ExerSense::ExerDisplay::singleton_ = nullptr;
ExerSense::ExerDisplay *ExerSense::ExerDisplay::GetInstance()
{
  if (singleton_ == nullptr)
  {
    singleton_ = new ExerDisplay();
  }
  return singleton_;
}

#endif