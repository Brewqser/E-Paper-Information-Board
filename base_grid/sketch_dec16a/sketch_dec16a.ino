#include <GxEPD.h>
#include <GxFont_GFX.h>
#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

//#include <Fonts/FreeMonoBold9pt7b.h>
//#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
//#include <Fonts/FreeMonoBold24pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI, /*CS=5*/ 10, /*DC=*/ 9, /*RST=*/ 8);
GxEPD_Class display(io, /*RST=*/ 8, /*BUSY=*/ 7);

int numberOfRows = 3;
int heightOffset = 75;
int widhtOffset = 300;
int widht2 = 300;
int spacing = 85;

String normalTimes[6] = {"12:00", "15:00", "11:00", "19:00", "9:00", "14:53"};
String presentTimes[6] = {"12:00", "15:00", "11:00", "16:29", "12:23", "14:52"};

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
  
  delay(1000);
}

void updateDisplay()
{
  display.fillScreen(GxEPD_WHITE);
 
  display.fillRect(300, 75, 300, 3, GxEPD_BLACK);
  for (int i=1;i<=numberOfRows;i++)
  {
      display.fillRect(widhtOffset, 75 + i * 85 , widht2, 3, GxEPD_BLACK);
  }
  display.fillRect(widhtOffset, heightOffset, 3, numberOfRows * 85, GxEPD_BLACK);
  display.fillRect(widhtOffset + widht2 / 2, heightOffset, 3, numberOfRows * 85, GxEPD_BLACK);
  display.fillRect(widhtOffset + widht2, heightOffset, 3, numberOfRows * 85, GxEPD_BLACK);

  display.setFont(&FreeMonoBold18pt7b);
  for (int i=0;i<numberOfRows;i++)
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
  
  display.update();
}
