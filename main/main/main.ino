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
int heightOffset = 147;
int widhtOffset = 40;
int widht2 = 300;
int spacing = 85;
int NFCcounter = 0;
int code1 = 2137;

String normalTimes[6] = {"12:00", "15:00", "11:00", "19:00", "09:00", "14:53"};
String presentTimes[6] = {"12:00", "15:00", "11:00", "16:29", "12:23", "14:52"};
String news[7] = {"Info", "InfoInfo", "InfoInfoInfo", "InfoInfoInfo", "InfoInfoInfoInfo", "InfoInfoInfoInfoInfo", "InfoInfoInfoInfoInfoInfo"};
String studentID[100];

String text_write = ""; // Text to write in the tag
char text[1000] = "";
char text_readp[1000] = {'\0'};                             // Text read in the tag
char text_readn[1000] = {'\0'};

int disrefresh = 0;
  
void setup()
{
  Serial.begin(115200);
  Serial.println();
  //Serial.println("setup");
  
  dev_i2c.begin();
  display.init(115200); // enable diagnostic output on Serial

  Serial.println("setup done");
  text_write.toCharArray(text, 1000);
  
  if(nfcTag.writeTxt(text) == false) {
    SerialPort.println("Write failed!");
    while(1);
  }
/*
  if(nfcTag.begin(NULL) == 0) {
    SerialPort.println("System Init done!");
  } else {
    SerialPort.println("System Init failed!");
    while(1);
  }
  */
  updateDisplay();
}

void loop() {
  
  checkNFC();

  if (disrefresh == 1) 
  {
    disrefresh = 0;

    SerialPort.println("update");
    updateDisplay();
  }
  delay(5000);
}







void parseNFC()
{
  String a(text_readn);

  int it=0;
  int itt=0;
  //SerialPort.println(a);

 
  String op = a.substring(0,1);
  
  if (op == "N")
  {
    String code = a.substring(2,6);
    //SerialPort.println("op: ");
    //SerialPort.println(op);
    //SerialPort.println("code: ");
    //SerialPort.println(code);
    //SerialPort.println(code1);
    if (code == String(code1))
    {
      op = a.substring(7,8);

      //if (op == "3") itt = 3; 
      //if (op == "2") itt = 2; 
      //if (op == "1") itt = 1; 

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
        for (int i=0; i < itt * 2;i++)
        {
           code = a.substring(it + i * 6, it + i * 6 + 5);
          // SerialPort.println(code);
           // SerialPort.println(normalTimes[i]);
           normalTimes[i] = code;
          SerialPort.println(normalTimes[i]);
        }
  
        for (int i=itt * 2; i < itt*4; i++)
        {
           code = a.substring(it + i * 6, it + i * 6 + 5);
           //SerialPort.println(presentTimes[i-itt * 2]); 
           presentTimes[i-itt * 2] = code;
           SerialPort.println(presentTimes[i-itt * 2]);
        }
        
        SerialPort.println("git");
        disrefresh = 1;
      }
      
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

        
    }
      
  }
  if (op == "S") 
  {
     SerialPort.println("student");
     String code = a.substring(2,8);
     studentID[NFCcounter] = code;
     disrefresh = 1;
     NFCcounter++;
  }

  text_write = "";
  for(int i=0; i< NFCcounter; i++)
    text_write = text_write + " " + studentID[i];
   text_write.toCharArray(text, 1000);
   SerialPort.println(nfcTag.writeTxt(text));
  nfcTag.writeTxt(text);
  

  
}

void checkNFC()
{
  nfcTag.readTxt(text_readn);
  SerialPort.print("Message content: ");
  SerialPort.println(text_readn);

  if(strcmp(text_readp, text_readn) == 0) {
    SerialPort.println("same");
    
  } else {
    SerialPort.println("difrent");
    parseNFC();
  }
  for (int i=0;i<255;i++)
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
  display.println(NFCcounter);
  
  
  display.update();
}
