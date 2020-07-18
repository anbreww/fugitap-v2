/**
 * Display.h
 * 
 * Helper class to handle everything related to the beer display
 * Showing beer names, status bar, fill meter, backlight level
 * 
 */

#include <TFT_eSPI.h>
#include "GfxUi.h"
#include "Beer.h"

class Display {
  public:
    Display(uint8_t backlight_pin, uint8_t ledc_chan, 
            TFT_eSPI * tft, GfxUi * ui, Beer * beer);
    void drawBeerScreen();
    void drawFillMeter();
    void setBacklight(uint8_t level);
    void writeStatusBar(const char * status, uint16_t text_color, bool force);
    void writeStatusBar(const char * status, uint16_t text_color);
    void begin(void);
  private:
    TFT_eSPI * _tft;
    GfxUi * _ui;
    Beer * _beer;
    void init(void);
};