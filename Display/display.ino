#include <Wire.h>

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
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2


const byte displayAddress = 0x3C;


void setup() {
delay(2000);
  Serial.begin(115200);
  Wire.begin();
  Serial.println("\nI2C Scanner");
  initDisplay();
   Serial.println("init done");
}


void loop() {
 

  delay(1000);           // wait 5 seconds for next scan
 Serial.println("ggg");
  displayCommand(SSD1306_INVERTDISPLAY);
}

void initDisplay(){
  Serial.println("init");
          displayCommand(SSD1306_DISPLAYOFF);                    // 0xAE
            Serial.println("init");
    displayCommand(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    displayCommand(0x80);                                  // the suggested ratio 0x80
    displayCommand(SSD1306_SETMULTIPLEX);                  // 0xA8
    displayCommand(0x1F);
    displayCommand(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    displayCommand(0x0);                                   // no offset
    displayCommand(SSD1306_SETSTARTLINE | 0x0);            // line #0
    displayCommand(SSD1306_CHARGEPUMP);                    // 0x8D
    displayCommand(0x14); 
    displayCommand(SSD1306_MEMORYMODE);                    // 0x20
    displayCommand(0x00);                                  // 0x0 act like ks0108
    displayCommand(SSD1306_SEGREMAP | 0x1);
    displayCommand(SSD1306_COMSCANDEC);
    displayCommand(SSD1306_SETCOMPINS);                    // 0xDA
    displayCommand(0x02);
    displayCommand(SSD1306_SETCONTRAST);                   // 0x81
    displayCommand(0x8F);
    displayCommand(SSD1306_SETPRECHARGE);                  // 0xd9
    displayCommand(0xF1); 
    displayCommand(SSD1306_SETVCOMDETECT);                 // 0xDB
    displayCommand(0x40);
    displayCommand(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    displayCommand(SSD1306_NORMALDISPLAY);      
    displayCommand(SSD1306_DISPLAYON);      
  }

void displayCommand(int command){
   int control = 0x00;   // Co = 0, D/C = 0
    Wire.beginTransmission(displayAddress);
    Serial.println("ggg");
    Wire.write(control);
    Serial.println("ggg");
    Wire.write(command);
    Serial.println("ggg");
    unsigned int kkk = Wire.endTransmission();
     Serial.println(kkk);
    Serial.println("kkk");
  } 

  void displayData(byte command){
   byte control = 0x40;   // Co = 0, D/C = 1
    Wire.beginTransmission(displayAddress);
    Wire.write(control);
    Wire.write(command);
    Wire.endTransmission();
  } 
