#include <GxEPD.h>
#include "IMG_0001.h"
#include <GxFont_GFX.h>
#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

#include GxEPD_BitmapExamples

//#include <Fonts/FreeMonoBold9pt7b.h>
//#include <Fonts/FreeMonoBold12pt7b.h>
//#include <Fonts/FreeMonoBold18pt7b.h>
//#include <Fonts/FreeMonoBold24pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI, /*CS=5*/ 10, /*DC=*/ 9, /*RST=*/ 8);
GxEPD_Class display(io, /*RST=*/ 8, /*BUSY=*/ 7);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");

  display.init(115200); // enable diagnostic output on Serial

  Serial.println("setup done");
}

void loop() {
  display.drawBitmap(epaperBackground, sizeof(epaperBackground));
  delay(1000);
  display.drawBitmap(gImage_7in5b_b, sizeof(gImage_7in5b_b));
  delay(1000);

}
