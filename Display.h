#ifndef Display_h
#define Display_h

#include "Config.h"
#include "pin.h"
#include <Adafruit_SSD1306.h>

class Display: public Adafruit_SSD1306
{
  public:
    Display(int resetPin) : Adafruit_SSD1306(resetPin) {}

    void init();
    void setupMessage(byte pos, String title, String description);
    void update();
    void select();
    void back();
    void change(int change);
  private:
    void set();
    void setLine(int line, String title, String value);
    void setLine(int line, String title, long value, bool showPlus);
    void setLine(int line, String title, long value);
    void setLine(int line, String title);
    void setLine(int line);
    String intToStr(int number);

    byte level = 0;
    int page = 0;
    const byte PAGE_RANGE = 7;
    int subpage = 0;
    const byte SUBPAGE_RANGE[7] = {4, 12, 4, 3, 9, 11, 8};
    String runtime = "";
    String title = "";
    String line0 = "";
    String line1 = "";
    String line2 = "";
};

const unsigned char PROGMEM LOGO[] = {
  0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x31, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x20, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x60, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x10, 0x00, 0x40, 0x5e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x18, 0x00, 0xc0, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x3c, 0x00, 0x80, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x26, 0x01, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x23, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x61, 0x83, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0x86, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xc0, 0xc4, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x4c, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x80, 0x48, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x58, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x70, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x70, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x60, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0c, 0x00, 0x40, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x18, 0x00, 0xc0, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x80, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x30, 0x00, 0x80, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x01, 0x80, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x60, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x40, 0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xc0, 0x01, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x40, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x40, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x60, 0x00, 0x00, 0x00, 0x00, 0x61, 0x87, 0xf1, 0xfc, 0xfe, 0x0f, 0xef, 0xf0, 0x7f, 0x00,
  0x20, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xcf, 0xf3, 0xfc, 0xff, 0x1f, 0xef, 0xf8, 0xff, 0x00,
  0x30, 0x00, 0x00, 0x00, 0x00, 0x81, 0xdf, 0xf7, 0xfc, 0xff, 0xbf, 0xe7, 0xfd, 0xff, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x01, 0x81, 0xdc, 0x07, 0x00, 0x03, 0xb8, 0x00, 0x1d, 0xc0, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x03, 0x01, 0xd8, 0x07, 0x00, 0x03, 0xb8, 0x00, 0x1d, 0x80, 0x00,
  0x18, 0x00, 0x00, 0x00, 0x06, 0x01, 0xd8, 0x03, 0xfc, 0xff, 0xbf, 0xe7, 0xf9, 0x8f, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x04, 0x01, 0xd8, 0x07, 0xfc, 0xff, 0xbf, 0xe7, 0xf1, 0x8f, 0x00,
  0x0c, 0x00, 0x00, 0x00, 0x0c, 0x01, 0xd8, 0x07, 0x00, 0x03, 0xb8, 0x0f, 0xe1, 0x83, 0x80,
  0x04, 0x00, 0x00, 0x00, 0x08, 0x01, 0xdc, 0x07, 0x00, 0x03, 0xb8, 0x0f, 0xf9, 0xc3, 0x80,
  0x04, 0x00, 0x00, 0x00, 0x18, 0x01, 0xdf, 0xf7, 0xfc, 0xff, 0xbf, 0xee, 0x7d, 0xff, 0x00,
  0x06, 0x00, 0x00, 0x00, 0x10, 0x01, 0xcf, 0xf3, 0xfc, 0xff, 0x1f, 0xee, 0x1c, 0xff, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x80, 0x00, 0x00, 0xc0, 0x01, 0xfe, 0x07, 0xe1, 0xfc, 0x0f, 0xc3, 0xf8, 0xff, 0x00,
  0x00, 0x80, 0x00, 0x01, 0x80, 0x01, 0xff, 0x1f, 0xf1, 0xff, 0x1f, 0xf3, 0xf9, 0xff, 0x00,
  0x00, 0xc0, 0x00, 0x01, 0x00, 0x01, 0xff, 0xbf, 0xf9, 0xff, 0x3f, 0xf3, 0xfb, 0xff, 0x00,
  0x00, 0x60, 0x00, 0x03, 0x00, 0x00, 0x03, 0xbc, 0x38, 0x07, 0x78, 0x38, 0xe3, 0x80, 0x00,
  0x00, 0x20, 0x00, 0x06, 0x00, 0x00, 0x03, 0xb8, 0x1c, 0x07, 0x70, 0x38, 0xe3, 0x80, 0x00,
  0x00, 0x30, 0x00, 0x0c, 0x00, 0x00, 0xff, 0xb8, 0x1d, 0xff, 0x70, 0x18, 0xe1, 0xfe, 0x00,
  0x00, 0x18, 0x00, 0x08, 0x00, 0x01, 0xff, 0x38, 0x1d, 0xff, 0x70, 0x18, 0xe1, 0xff, 0x00,
  0x00, 0x08, 0x00, 0x18, 0x00, 0x01, 0xf8, 0x38, 0x1c, 0x07, 0x70, 0x38, 0xe0, 0x07, 0x00,
  0x00, 0x0c, 0x00, 0x30, 0x00, 0x01, 0xfe, 0x3c, 0x1c, 0x07, 0x78, 0x38, 0xe0, 0x03, 0x80,
  0x00, 0x06, 0x00, 0x60, 0x00, 0x01, 0xdf, 0xbf, 0xf9, 0xff, 0x3f, 0xf8, 0xe1, 0xff, 0x80,
  0x00, 0x03, 0x00, 0x40, 0x00, 0x01, 0xc7, 0x9f, 0xf9, 0xff, 0x3f, 0xf0, 0xe1, 0xff, 0x00,
  0x00, 0x01, 0x01, 0xc0, 0x00, 0x01, 0xc1, 0x8f, 0xe1, 0xfe, 0x0f, 0xe0, 0xe1, 0xfe, 0x00,
  0x00, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif
