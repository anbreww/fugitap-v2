#include "Display.h"
#include "WebResource.h"

#define FONT_STATUS &FreeMono9pt7b
#define FONT_LABELS &FreeMonoBold9pt7b
#define FONT_BEER &FreeMonoBold12pt7b
#define FONT_STATS &FreeMono12pt7b

#define NUM_LINES 4
#define MARGIN 10
const uint16_t line_pos[] = {28, 100, 170, 265};
const uint8_t sp_top = 5; // space between line and text on top
const uint8_t sp_bot = 7; // space between line and text below

#define LBL_FONT 2
#define STAT_FONT 4

WebResource webResource;
void downloadCallback(String filename, int16_t bytesDownloaded, int16_t bytesTotal);
ProgressCallback _downloadCallback = downloadCallback;


Display::Display(uint8_t backlight_pin, uint8_t ledc_chan, TFT_eSPI * tft, GfxUi * ui, Beer * beer)
{
  _tft = tft;
  _ui = ui;
  _beer = beer;
}

void Display::drawBeerScreen()
{
  _tft->fillScreen(TFT_BLACK);
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  const uint8_t glass_w = 70;
  const uint8_t glass_h = 80;
  uint16_t end_pos;

  // draw divider lines first
  for (uint8_t i = 0; i < NUM_LINES; i++)
  {
    if (i == 2)
    {
      end_pos = TFT_WIDTH - MARGIN - glass_w;
    }
    else
    {
      end_pos = TFT_WIDTH - MARGIN;
    }
    _tft->drawLine(MARGIN, line_pos[i], end_pos, line_pos[i], TFT_WHITE);
  }

  // stats are positioned below the lines
  _tft->setTextDatum(TL_DATUM);
  _tft->setFreeFont(FONT_BEER);
  _tft->setTextWrap(true);
  //_tft->drawString(_beer->name(), MARGIN, line_pos[0] + sp_bot);

  // labels are positioned above the lines
  _tft->setTextDatum(BL_DATUM);
  _tft->setFreeFont(FONT_LABELS);
  _tft->drawString("BEER", MARGIN, line_pos[0] - sp_top);
  _tft->drawString("STYLE", MARGIN, line_pos[1] - sp_top);
  _tft->drawString("STATS", MARGIN, line_pos[2] - sp_top);
  _tft->drawString("FILL", MARGIN, line_pos[3] - sp_top);

  // stats are positioned below the lines
  _tft->setTextDatum(TL_DATUM);
  _tft->setFreeFont(FONT_BEER);
  _tft->setTextWrap(true);
  //String beername = "Shoop da whoop";
  //String beername = Firebase.getString("fugidaire/v2/beers/beer1/name");
  //tft.drawString(beername, MARGIN, line_pos[0] + sp_bot);
  _tft->drawString(_beer->name(), MARGIN, line_pos[0] + sp_bot);
  //tft.drawString(beer_name[1], MARGIN, line_pos[0] + sp_bot + 20);
  _tft->setFreeFont(FONT_STATS);
  _tft->drawString(_beer->type(), MARGIN, line_pos[1] + sp_bot);
  _tft->setTextWrap(false);
  _tft->drawString("ABV: " + _beer->abv(), MARGIN, line_pos[2] + sp_bot);
  _tft->drawString("IBU: " + _beer->ibu(), MARGIN, line_pos[2] + sp_bot + 20);
  _tft->drawString("OG : " + _beer->og(), MARGIN, line_pos[2] + sp_bot + 40);

  // Placeholder for glass which will be BMP / JPEG / PNG
  if (WiFi.isConnected())
  {
    String dl_url = String("http://elementary.ch") + _beer->glass_img();
    webResource.downloadFile(dl_url, _beer->glass_img(), _downloadCallback);
  }

  if (SPIFFS.exists(_beer->glass_img()) == true)
  {
    _ui->drawBmp(_beer->glass_img(), 240 - MARGIN - glass_w, line_pos[2] - 15);
  }
  else
  {
    //tft.drawRect(240 - MARGIN - glass_w, line_pos[2] + sp_bot, glass_w, glass_h, TFT_WHITE);
    _tft->drawRect(240 - MARGIN - glass_w, line_pos[2] - 15, glass_w, glass_h, TFT_WHITE);
    _tft->setTextDatum(MC_DATUM);
    _tft->drawString("glass.png", 240 - MARGIN - glass_w / 2, line_pos[2] + sp_bot + glass_h / 2, LBL_FONT);
  }
}


void Display::writeStatusBar(const char * status, uint16_t text_color)
{
  writeStatusBar(status, text_color, false);
}

void Display::writeStatusBar(const char * status, uint16_t text_color, bool force)
{
  _tft->setTextDatum(BC_DATUM);
  _tft->setFreeFont(FONT_STATUS);
  _tft->setTextColor(text_color);
  _tft->fillRect(0, 300, 240, 20, TFT_BLACK);
  if (millis() < 60000 || force)
  {
    _tft->drawString(status, 120, 318);
  }
}

void Display::setBacklight(uint8_t level)
{
  ;
}

void Display::drawFillMeter(void)
{
    // draw fill meter, capacity remaining and flow rate below
  // TODO : pass a beer object on init so we can grab info from it.
  double litres = 17.5;

  //Serial.println("Litres: " + String(beer.volume()) + " full : " + String(beer.full_vol()));

  int8_t fill_percent = (uint8_t) litres * 100 / 19.0; // was : beer.full_vol
  if (fill_percent > 100) {
      fill_percent = 100;
  }
  if (litres < 0) {
      fill_percent = 0;
  }

  uint16_t fill_color = TFT_GREEN;
  if (fill_percent < 20) {
      fill_color = TFT_RED;
  } else if (fill_percent < 30) {
      fill_color = TFT_ORANGE;
  } else if (fill_percent < 40) {
      fill_color = TFT_YELLOW;
  }

  _tft->fillRect(240-95, line_pos[3] + sp_bot, 240, 20, TFT_BLACK);
  _ui->drawProgressBar(MARGIN, line_pos[3] + sp_bot, 240-100, 20, fill_percent,
      TFT_WHITE, fill_color);

  _tft->setTextDatum(TR_DATUM);
  _tft->setFreeFont(FONT_STATUS);
  _tft->setTextColor(TFT_WHITE);
  _tft->drawString(String(litres) + "l", TFT_WIDTH-MARGIN, line_pos[3] + sp_bot);

  //analogWrite(PIN_BACKLIGHT, PWMRANGE*fill_percent/100);
}

void Display::begin(void)
{
  init();
}

void Display::init(void)
{
  _tft->begin();
  _tft->fillScreen(TFT_BLACK);
  _tft->setRotation(0);

  _tft->setFreeFont(FONT_STATUS);

  _tft->setTextDatum(BC_DATUM);
  _tft->drawString("FugiTap V2", 120, 140);
  _tft->drawString("Andrew Watson - 2020", 120, 160);
}

void downloadCallback(String filename, int16_t bytesDownloaded, int16_t bytesTotal)
{
    Serial.println("Downloading " + filename + " " + String(bytesDownloaded) + "/" + String(bytesTotal));
    yield(); // feed the watchdog
}