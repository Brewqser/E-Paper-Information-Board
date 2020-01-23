#include <GxEPD.h>
#include <GxFont_GFX.h>
#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

//#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include <Wire.h>
#include <M24SR.h>

#define SerialPort      Serial
#define I2C2_SCL        PB10
#define I2C2_SDA        PB11
#define M24SR_ADDR      0xAC
#define GPO_PIN         PE4
#define RF_DISABLE_PIN  PE2

TwoWire dev_i2c(I2C2_SDA, I2C2_SCL);
M24SR nfcTag(M24SR_ADDR, &dev_i2c, NULL, GPO_PIN, RF_DISABLE_PIN);

GxIO_Class io(SPI, /*CS=5*/ 10, /*DC=*/ 9, /*RST=*/ 8);
GxEPD_Class display(io, /*RST=*/ 8, /*BUSY=*/ 7);

int numberOfHours = 3;
int numberOfNews = 7;
int heightOffset = 145;
int widhtOffset = 5;
int widht2 = 371;
int spacing = 85;
int NFCcounter = 0;
int code1 = 2137;

String normalTimes[9] = {"MON", "12:00", "15:00","TUE", "11:00", "19:00","FRI", "09:00", "14:53"};
String presentTimes[9] = {"MON", "12:00", "15:00", "WED", "11:00", "16:29","FRI", "12:23", "14:52"};
String news[7] = {"", "", "", "", "", "", ""};
String studentID[100];

String text_write = "Brak"; // Text to write in the tag
char text[1000] = "Brak";
char text_readp[1000] = {'\0'};                             // Text read in the tag
char text_readn[1000] = {'\0'};

int disrefresh = 0;
  
void setup()
{
  Serial.begin(115200);
  Serial.println();
  
  dev_i2c.begin();
  display.init(115200); // enable diagnostic output on Serial
  
  text_write.toCharArray(text, 1000);
  
  if(nfcTag.writeTxt(text) == false) {
    SerialPort.println("Write failed!");
    while(1);
  }
  
  updateDisplay();
}

void loop() {
  
  checkNFC();

  if (disrefresh == 1) 
  {
    disrefresh = 0;
    updateDisplay();
  }
  delay(5000);
}







void parseNFC()
{
  String a(text_readn);

  int it=0;
  int itt=0;
 
  String op = a.substring(0,1);
  
  if (op == "N")
  {
    String code = a.substring(2,6);
    if (code == String(code1))
    {
      op = a.substring(7,8);
      if (op == "R")
      {
        NFCcounter = 0;
        disrefresh = 1;
      }
      else if (op == "H")
      {
        op = a.substring(9,10);
        
        if (op == "3") itt = 3; 
        if (op == "2") itt = 2; 
        if (op == "1") itt = 1; 
        
        numberOfHours = itt;
        it=11;
        for (int i=0; i < itt;i++)
        {
           code = a.substring(it + i * 16, it + i * 16 + 3);
           normalTimes[i*3] = code;

           code = a.substring(it + i * 16 + 4, it + i * 16 + 9);
           normalTimes[i*3 + 1] = code;

           code = a.substring(it + i * 16 + 10, it + i * 16 + 15);
           normalTimes[i*3 + 2] = code;
        }
        it=11 + itt * 16;
        for (int i=0; i < itt; i++)
        {
           code = a.substring(it + i * 16, it + i * 16 + 3);
           presentTimes[i*3] = code;

           code = a.substring(it + i * 16 + 4, it + i * 16 + 9); 
           presentTimes[i*3 + 1] = code;

           code = a.substring(it + i * 16 + 10, it + i * 16 + 15);
           presentTimes[i*3 + 2] = code;
        }
        disrefresh = 1;
      }
      else if (op == "I")
      {
          op = a.substring(9,10);
          if (op == "7") itt = 7; 
          if (op == "6") itt = 6; 
          if (op == "5") itt = 5; 
          if (op == "4") itt = 4;
          if (op == "3") itt = 3; 
          if (op == "2") itt = 2; 
          if (op == "1") itt = 1;
          it=11;
          code = a.substring(it, max(it + 24, int(a.length())));
          news[itt-1] = code;
          disrefresh = 1;
      }
    }
  }
  if (op == "S") 
  {
     String code = a.substring(2,8);
     studentID[NFCcounter] = code;
     disrefresh = 1;
     NFCcounter++;
  }

  if(NFCcounter == 0)
  {
    text_write = "Brak";
    text_write.toCharArray(text, 1000);
  }
  else
  {
    text_write = "";
    for(int i=0; i< NFCcounter; i++)
    {
      text_write = text_write + " " + studentID[i];
    }
    text_write.toCharArray(text, 1000);
  }
  nfcTag.writeTxt(text);
}

void checkNFC()
{
  nfcTag.readTxt(text_readn);
  SerialPort.print("Message content: ");
  SerialPort.println(text_readn);

  if(strcmp(text_readp, text_readn) != 0)
    parseNFC();
    
  for (int i=0;i<1000;i++)
    text_readp[i] = text_readn[i];
}

void updateDisplay()
{
  display.fillScreen(GxEPD_WHITE);

  display.setRotation(3);

  // godziny
 
  display.fillRect(widhtOffset, heightOffset, widht2, 3, GxEPD_BLACK);
  for (int i=1;i<=numberOfHours;i++)
  {
      display.fillRect(widhtOffset, heightOffset + i * 85 , widht2 + 3, 3, GxEPD_BLACK);
  }
  display.fillRect(widhtOffset, heightOffset, 3, numberOfHours * 85, GxEPD_BLACK);
  display.fillRect(widhtOffset + 100, heightOffset, 3, numberOfHours * 85, GxEPD_BLACK);
  display.fillRect(widhtOffset + 100 + ((widht2 - 100) / 2), heightOffset, 3, numberOfHours * 85, GxEPD_BLACK);
  display.fillRect(widhtOffset + widht2, heightOffset, 3, numberOfHours * 85, GxEPD_BLACK);

  display.setFont(&FreeMonoBold18pt7b);
  for (int i=0;i<numberOfHours;i++)
  {
      display.setCursor(widhtOffset + 15, heightOffset + 15 + (i + 0.5) * spacing);
      if (normalTimes[3*i] == presentTimes[3*i]) display.setTextColor(GxEPD_BLACK);
      else display.setTextColor(GxEPD_RED);
      display.println(presentTimes[3*i]);

      display.setCursor(widhtOffset + 100 + 15, heightOffset + 15 + (i + 0.5) * spacing);
      if (normalTimes[3*i+1] == presentTimes[3*i+1]) display.setTextColor(GxEPD_BLACK);
      else display.setTextColor(GxEPD_RED);
      display.println(presentTimes[3*i+1]);

      display.setCursor(widhtOffset + 100 + ((widht2 - 100) / 2) + 15, heightOffset + 15 + (i + 0.5) * spacing);
      if (normalTimes[3*i+2] == presentTimes[3*i+2]) display.setTextColor(GxEPD_BLACK);
      else display.setTextColor(GxEPD_RED);
      display.println(presentTimes[3*i+2]);
  }

  // news
  
  display.fillRect(0, 415, 384, 30, GxEPD_BLACK);
  display.setCursor(150, 440);
  display.setFont(&FreeMonoBold18pt7b);
  display.setTextColor(GxEPD_WHITE);
  display.println("NEWS"); 

  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);
  for (int i=0;i<numberOfNews;i++)
  {
      display.setCursor(5, 470 + (i * 24));
      display.println(news[i]);
  }


  // NFC counter

  display.fillRect(0, 120, 384, 10, GxEPD_BLACK);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold24pt7b);
  display.setCursor(100, 70);
  display.println("NFC: ");
  display.setCursor(230, 70);
  display.println(NFCcounter);
  
  
  display.update();
}
