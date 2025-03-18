#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_SHT31.h" 
#include "Arduino.h"
#include "AudioFileSourceLittleFS.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"
#include "FS.h"
#include "LittleFS.h"
#include <HardwareSerial.h>

HardwareSerial VC02(1);  // Use UART1 (pins can be reassigned)

// Define UART Pins
#define VC02_TX 17  // ESP32 TX -> VC-02 RX
#define VC02_RX 16  // ESP32 RX -> VC-02 TX


Adafruit_SHT31 sht31 = Adafruit_SHT31();

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// I2S Pins (MAX98357A)
#define I2S_BCLK  25  
#define I2S_LRC   26  
#define I2S_DOUT  27  

// Audio components
AudioFileSourceLittleFS *file;
AudioGeneratorWAV *wav;
AudioOutputI2S *out;

unsigned long delayTime = 1000;
unsigned long delayTime2 = 3000;

float temp = 0;
float humid = 0;
uint16_t u16temp = 0;
uint16_t u16humid = 0;

bool isPlaying = false;  // Global flag for audio playback

uint8_t u8index = 0;
uint8_t receiveBuffer[15] ={};

void playFile(const char *filename) {
    Serial.printf("Playing: %s\n", filename);

    file = new AudioFileSourceLittleFS(filename);
    wav = new AudioGeneratorWAV();
    wav->begin(file, out);

    while (wav->isRunning()) {
        wav->loop(); // Keep playing
    }

    delete wav;
    delete file;
    Serial.printf("Finished: %s\n", filename);
}

void playOneToNine(uint8_t number)
{
  switch(number)
  {
    case 9:
    {
      playFile("/9.wav");
      break;
    }
    case 8:
    {
      playFile("/8.wav");
      break;
    }
    case 7:
    {
      playFile("/7.wav");
      break;
    }
    case 6:
    {
      playFile("/6.wav");
      break;
    }
    case 5:
    {
      playFile("/5.wav");
      break;
    }
    case 4:
    {
      playFile("/4.wav");
      break;
    }
    case 3:
    {
      playFile("/3.wav");
      break;
    }
    case 2:
    {
      playFile("/2.wav");
      break;
    }
    case 1:
    {
      playFile("/1.wav");
      break;
    }
    default:
    {
      playFile("/0.wav");
      break;
    }
  }
}

void announcement(uint16_t u16temp,uint8_t tens,uint8_t ones) {
  if (u16temp >= 999) {  // Handle numbers above 100
      playFile("/90.wav");
      playFile("/9.wav");
      playFile("/point.wav");
      playFile("/9.wav");
  }
  else
  {
    if ((u16temp/10 % 100) >= 11 && (u16temp/10 % 100) <= 19) {
      // Handle special cases for 11 to 19
      char filename[10];
      sprintf(filename, "/%d.wav", u16temp % 100);
      playFile(filename);
    } else {
        // Play tens (20, 30, ..., 90)
        if (tens > 1) {
            char filename[10];
            sprintf(filename, "/%d0.wav", tens);
            playFile(filename);
        }

        // Play ones (1, 2, ..., 9)
        if (ones > 0) {
            playOneToNine(ones);
        }
    }

    playFile("/point.wav");
    playOneToNine(u16temp % 10);
  }
}

void updateDisplay(float temp, float humid) {
  display.clearDisplay();
    
  // display temperature
  display.setTextSize(1);

  display.setTextSize(2);
  display.setCursor(0,18);
  display.print(temp);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  
  // display humidity
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(humid);
  display.print("%");
  display.setTextSize(1);
  display.setCursor(74, 50);
  display.print(" ");
  display.print("Rel H");  
  display.display(); 
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Hellow World!"));
  VC02.begin(115200, SERIAL_8N1, VC02_RX, VC02_TX);  // UART to VC-02

  if (!LittleFS.begin(true)) {
      Serial.println("LittleFS Mount Failed!");
  }

  Serial.printf("Total LittleFS Space: %d bytes\n", LittleFS.totalBytes());
  Serial.printf("Used Space: %d bytes\n", LittleFS.usedBytes());

  out = new AudioOutputI2S();
  out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  out->SetGain(0.1);  
    
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
  }

  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
      Serial.println("Couldn't find SHT31");
  }

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Hello, world!");
  display.display(); 
}


void loop() {
  delay(1);
  if( 0 != delayTime)
  {
    delayTime--;
    if( 0 == delayTime)
    {
      delayTime = 1000;
      temp = sht31.readTemperature();
      humid = sht31.readHumidity();
      if (! isnan(temp)) {  // check if 'is not a number'
        Serial.print("Temp *C = "); Serial.println(temp);
      }
      else {    
        temp=0.0;
        Serial.println("Failed to read temperature");
      }
        
      if (! isnan(humid)) {  // check if 'is not a number'
        Serial.print("Hum. % = "); Serial.println(humid);
      }
      else { 
        humid=0.0;
        Serial.println("Failed to read humidity");    
      }
      Serial.println();
      updateDisplay(temp,humid);
    }
  }
  if (VC02.available()) {
      u8index = 0 ;
      Serial.print("Received: ");
      while (VC02.available()) {
          receiveBuffer[u8index++] = VC02.read();
          Serial.printf("0x%02X ", receiveBuffer[u8index-1]);  // Print in HEX
          if(u8index >= 15)
          {
            u8index = 0 ;
          }
      }
      Serial.println();  // New line after data received
      if( 5 == u8index )
      {
        if( 0x5A == receiveBuffer[0]&&
            0x00 == receiveBuffer[1]&&
            0x00 == receiveBuffer[2]&&
            0x00 == receiveBuffer[3]&&
            0x5A == receiveBuffer[4])
        {
          playFile("/hi.wav");
        }
        if( 0x5A == receiveBuffer[0]&&
            0x00 == receiveBuffer[1]&&
            0x00 == receiveBuffer[2]&&
            0x01 == receiveBuffer[3]&&
            0x5A == receiveBuffer[4])
        {
          uint16_t u16temp = (uint16_t)(temp * 10);  // Convert to 1 decimal place
          uint8_t tens = (u16temp / 100) % 10;        // Extract tens place
          uint8_t ones = (u16temp/10) % 10;               // Extract ones place

          playFile("/temp.wav");
          announcement(u16temp,tens,ones);
        }
        else if( 0x5A == receiveBuffer[0]&&
            0x00 == receiveBuffer[1]&&
            0x00 == receiveBuffer[2]&&
            0x02 == receiveBuffer[3]&&
            0x5A == receiveBuffer[4])
        {
          uint16_t u16temp = (uint16_t)(humid * 10);  // Convert to 1 decimal place
          uint16_t tens = (u16temp / 100) % 10;        // Extract tens place
          uint16_t ones = (u16temp/10) % 10;               // Extract ones place

          playFile("/humid.wav");
          announcement(u16temp,tens,ones);
        }
        else if( 0x5A == receiveBuffer[0]&&
            0x00 == receiveBuffer[1]&&
            0x00 == receiveBuffer[2]&&
            0x03 == receiveBuffer[3]&&
            0x5A == receiveBuffer[4])
        {
          playFile("/siren.wav");
        }
      }
    }
  // if( 0 != delayTime2)
  // {
  //   delayTime2--;
  //   if( 0 == delayTime2)
  //   {
  //     delayTime2 = 10000;
  //     uint16_t u16temp = (uint16_t)(temp * 10);  // Convert to 1 decimal place
  //     uint8_t tens = (u16temp / 100) % 10;        // Extract tens place
  //     uint8_t ones = (u16temp/10) % 10;               // Extract ones place

  //     playFile("/temp.wav");
  //     announcement(u16temp,tens,ones);

  //     u16temp = (uint16_t)(humid * 10);  // Convert to 1 decimal place
  //     tens = (u16temp / 100) % 10;        // Extract tens place
  //     ones = (u16temp/10) % 10;               // Extract ones place

  //     playFile("/humid.wav");
  //     announcement(u16temp,tens,ones);
  //   }
  // }
}


