#include <GxEPD.h>
#include <GxFont_GFX.h>
#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

//#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI, /*CS=5*/ 10, /*DC=*/ 9, /*RST=*/ 8);
GxEPD_Class display(io, /*RST=*/ 8, /*BUSY=*/ 7);

int numberOfHours = 3;
int numberOfNews = 7;
int heightOffset = 147;
int widhtOffset = 40;
int widht2 = 300;
int spacing = 85;
int NCFcounter = 100;

String normalTimes[6] = {"12:00", "15:00", "11:00", "19:00", "9:00", "14:53"};
String presentTimes[6] = {"12:00", "15:00", "11:00", "16:29", "12:23", "14:52"};
String news[7] = {"Papier jest well ciekawy", "NWM co napisac", "aaaaaaaa", "jest 24", "i jeszcze", "nie skonczylem", "xd"};

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");

  display.init(115200); // enable diagnostic output on Serial

  Serial.println("setup done");
}

void loop() {
  updateDisplay();
  Serial.println("asd");
  delay(1000);
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
  display.fillRect(widhtOffset + widht2 / 2, heightOffset, 3, numberOfHours * 85, GxEPD_BLACK);
  display.fillRect(widhtOffset + widht2, heightOffset, 3, numberOfHours * 85, GxEPD_BLACK);

  display.setFont(&FreeMonoBold18pt7b);
  for (int i=0;i<numberOfHours;i++)
  {
      display.setCursor(widhtOffset + 15, heightOffset + 15 + (i + 0.5) * spacing);
      if (normalTimes[2*i] == presentTimes[2*i]) display.setTextColor(GxEPD_BLACK);
      else display.setTextColor(GxEPD_RED);
      display.println(presentTimes[2*i]);

      display.setCursor(widhtOffset + widht2 / 2 + 15, heightOffset + 15 + (i + 0.5) * spacing);
      if (normalTimes[2*i+1] == presentTimes[2*i+1]) display.setTextColor(GxEPD_BLACK);
      else display.setTextColor(GxEPD_RED);
      display.println(presentTimes[2*i+1]);
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
  display.println(NCFcounter);
  
  
  display.update();
}
