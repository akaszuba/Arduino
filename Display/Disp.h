#ifndef _DISP_H_
#define _DISP_H_
#define SSD1306_LCDHEIGHT 32
#define SSD1306_LCDWIDTH 128
#define SSD1306_DISPLAY_ADDRESS 0x3C

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2


class Disp {
public:
Disp();
void begin();
void refresh();
void clear();
void drawBitmap(uint8_t bitmap[], uint8_t bitmapWidth, uint8_t bitmapHeight, uint8_t offsetX, uint8_t offsetY);
//void writeChar(char c, offsetX, offsetY);

private:
void displayCommand(uint8_t command);
void displayData(uint8_t command);
void initDisplay();
//uint8_t *buffer;
};
#endif