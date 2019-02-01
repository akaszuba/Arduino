#include <Wire.h>
#include <stdlib.h>
#include "Disp.h"

static uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8];

Disp::Disp()
{
}

void Disp::begin()
{

  Wire.begin();
  initDisplay();
}

void Disp::clear()
{
  memset(buffer, 0, (SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8));
}

void Disp::drawBitmap(uint8_t bitmap[], uint8_t bitmapWidth, uint8_t bitmapHeight, uint8_t offsetX, uint8_t offsetY)
{
  //void applyBitmap(int bitmap[], int bitmapSizeX, int bitmapSizeY, int offsetX, int offsetY ){
  int memRow = offsetY / 8;
  int yOffsetMod = offsetY % 8;
  int bitmapMod = offsetY % 8;
  int bitmapRows = bitmapHeight / 8;
  for (int row = 0; row < bitmapRows; row++)
  {
    for (int i = 0; i < bitmapWidth; i++)
    {
      buffer[i + offsetX + (row * SSD1306_LCDWIDTH) + (memRow * SSD1306_LCDWIDTH)] |= bitmap[i + row * bitmapWidth] << yOffsetMod;
      if (bitmapMod > 0)
      {
        buffer[i + offsetX + ((row + 1) * SSD1306_LCDWIDTH) + (memRow * SSD1306_LCDWIDTH)] |= bitmap[i + row * bitmapWidth] >> (8 - yOffsetMod);
      }
    }
  }
}


void Disp::displayCommand(uint8_t command)
{
  Wire.beginTransmission(SSD1306_DISPLAY_ADDRESS);
  Wire.write(0x00);
  Wire.write(command);
  Wire.endTransmission();
}

void Disp::displayData(uint8_t command)
{
  Wire.beginTransmission(SSD1306_DISPLAY_ADDRESS);
  Wire.write(0x40);
  Wire.write(command);
  Wire.endTransmission();
}

void Disp::initDisplay()
{
  displayCommand(SSD1306_DISPLAYOFF);         // 0xAE
  displayCommand(SSD1306_SETDISPLAYCLOCKDIV); // 0xD5
  displayCommand(0x00);                       // the suggested ratio 0x00
  displayCommand(SSD1306_SETMULTIPLEX);       // 0xA8
  displayCommand(0x1F);
  displayCommand(SSD1306_SETDISPLAYOFFSET);   // 0xD3
  displayCommand(0x0);                        // no offset
  displayCommand(SSD1306_SETSTARTLINE | 0x0); // line #0
  displayCommand(SSD1306_CHARGEPUMP);         // 0x8D
  displayCommand(0x14);
  displayCommand(SSD1306_MEMORYMODE); // 0x20
  displayCommand(0x00);               // 0x0 act like ks0108
  displayCommand(SSD1306_SEGREMAP | 0x1);
  displayCommand(SSD1306_COMSCANDEC);
  displayCommand(SSD1306_SETCOMPINS); // 0xDA
  displayCommand(0x02);
  displayCommand(SSD1306_SETCONTRAST); // 0x81
  displayCommand(0x8F);
  displayCommand(SSD1306_SETPRECHARGE); // 0xd9
  displayCommand(0xF1);
  displayCommand(SSD1306_SETVCOMDETECT); // 0xDB
  displayCommand(0x40);
  displayCommand(SSD1306_DISPLAYALLON_RESUME); // 0xA4
  displayCommand(SSD1306_NORMALDISPLAY);
  displayCommand(SSD1306_DISPLAYON);
}

void Disp::refresh()
{
  displayCommand(SSD1306_COLUMNADDR);
  displayCommand(0);                    // Column start address (0 = reset)
  displayCommand(SSD1306_LCDWIDTH - 1); // Column end address (127 = reset)
  displayCommand(SSD1306_PAGEADDR);
  displayCommand(0); // Page start address (0 = reset)
  displayCommand(3); // Page end address

  for (uint16_t i = 0; i < (SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8); i++)
  {
    // send a bunch of data in one xmission
    Wire.beginTransmission(SSD1306_DISPLAY_ADDRESS);
    Wire.write(0x40);
    for (uint8_t x = 0; x < 16; x++)
    {
      Wire.write(buffer[i]);
      i++;
    }
    i--;
    Wire.endTransmission();
  }
}

Disp Display;
