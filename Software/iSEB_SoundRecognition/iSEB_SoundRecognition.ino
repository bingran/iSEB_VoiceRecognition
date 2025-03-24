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

// UART for VC-02 voice module
HardwareSerial VC02(1);  // Use UART1

// Define UART Pins for VC-02
#define VC02_TX 17  // ESP32 TX -> VC-02 RX
#define VC02_RX 16  // ESP32 RX -> VC-02 TX

// Temperature & Humidity sensor
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// OLED Display Configuration
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// I2S Audio Output Pins (MAX98357A)
#define I2S_BCLK  25  
#define I2S_LRC   26  
#define I2S_DOUT  27  

// Audio components
AudioFileSourceLittleFS *file;
AudioGeneratorWAV *wav;
AudioOutputI2S *out;

// Global variables
unsigned long delayTime = 1000;
float f32Temp = 0.0;
float f32Humid = 0.0;
uint16_t u16Temp = 0;
uint16_t u16Humid = 0;

// Buffer for UART communication
uint8_t u8Index = 0;
uint8_t u8ReceiveBuffer[15] = {};

/**
 * @brief Plays a WAV file from LittleFS.
 * @param filename Path to the WAV file.
 */
void PlayFile(const char *filename) {
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

/**
 * @brief Plays number audio files (1-9).
 * @param u8Number The number to be spoken.
 */
void PlayOneToNine(uint8_t u8Number) {
    switch (u8Number) {
        case 9: PlayFile("/9.wav"); break;
        case 8: PlayFile("/8.wav"); break;
        case 7: PlayFile("/7.wav"); break;
        case 6: PlayFile("/6.wav"); break;
        case 5: PlayFile("/5.wav"); break;
        case 4: PlayFile("/4.wav"); break;
        case 3: PlayFile("/3.wav"); break;
        case 2: PlayFile("/2.wav"); break;
        case 1: PlayFile("/1.wav"); break;
        default: PlayFile("/0.wav"); break;
    }
}

/**
 * @brief Announces temperature or humidity in spoken format.
 * @param u16Value The temperature/humidity value multiplied by 10.
 * @param u8Tens The tens place value.
 * @param u8Ones The ones place value.
 */
void AnnounceValue(uint16_t u16Value, uint8_t u8Tens, uint8_t u8Ones) {
    if (u16Value >= 999) {  
        PlayFile("/90.wav");
        PlayFile("/9.wav");
        PlayFile("/point.wav");
        PlayFile("/9.wav");
    } else {
        if ((u16Value / 10 % 100) >= 11 && (u16Value / 10 % 100) <= 19) {
            char filename[10];
            sprintf(filename, "/%d.wav", u16Value % 100);
            PlayFile(filename);
        } else {
            if (u8Tens > 1) {
                char filename[10];
                sprintf(filename, "/%d0.wav", u8Tens);
                PlayFile(filename);
            }
            if (u8Ones > 0) {
                PlayOneToNine(u8Ones);
            }
        }
        PlayFile("/point.wav");
        PlayOneToNine(u16Value % 10);
    }
}

/**
 * @brief Updates the OLED display with temperature and humidity values.
 */
void UpdateDisplay(float f32Temp, float f32Humid) {
    display.setTextColor(WHITE);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 18);
    display.print(f32Temp);
    display.print(" ");
    display.setTextSize(1);
    display.cp437(true);
    display.write(167);
    display.setTextSize(2);
    display.print("C");

    display.setCursor(0, 45);
    display.print(f32Humid);
    display.print("%");
    display.display();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hellow World!");

  VC02.begin(115200, SERIAL_8N1, VC02_RX, VC02_TX);   // UART communication setup

  if (!LittleFS.begin(true)) {
      Serial.println("LittleFS Mount Failed!");
  }

  Serial.printf("Total LittleFS Space: %d bytes\n", LittleFS.totalBytes());
  Serial.printf("Used Space: %d bytes\n", LittleFS.usedBytes());

  out = new AudioOutputI2S();
  out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  out->SetGain(0.1);  
    
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println("SSD1306 allocation failed");
  }

  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
      Serial.println("Couldn't find SHT31");
  }
}

void loop() {
  delay(1);
  if (0U != delayTime) {
      delayTime--;
      if (0U == delayTime) {
          delayTime = 1000;
          f32Temp = sht31.readTemperature();
          f32Humid = sht31.readHumidity();
          Serial.printf("Temp: %.1f C, Humidity: %.1f%%\n", f32Temp, f32Humid);
          UpdateDisplay(f32Temp, f32Humid);
      }
  }

  if (VC02.available()) {
    u8Index = 0;
    Serial.print("Received: ");
    while (VC02.available()) {
        u8ReceiveBuffer[u8Index++] = VC02.read();
        Serial.printf("0x%02X ", u8ReceiveBuffer[u8Index - 1]);
        if (15 == u8Index) {
            u8Index = 0;
        }
    }
    Serial.println();  // New line after data received
    if( 5 == u8Index )
    {
      if( 0x5A == u8ReceiveBuffer[0]&&
          0x00 == u8ReceiveBuffer[1]&&
          0x00 == u8ReceiveBuffer[2]&&
          0x00 == u8ReceiveBuffer[3]&&
          0x5A == u8ReceiveBuffer[4])
      {
        PlayFile("/hi.wav");
      }
      if( 0x5A == u8ReceiveBuffer[0]&&
          0x00 == u8ReceiveBuffer[1]&&
          0x00 == u8ReceiveBuffer[2]&&
          0x01 == u8ReceiveBuffer[3]&&
          0x5A == u8ReceiveBuffer[4])
      {
        uint16_t u16temp = (uint16_t)(f32Temp * 10);  // Convert to 1 decimal place
        uint8_t tens = (u16temp / 100) % 10;        // Extract tens place
        uint8_t ones = (u16temp/10) % 10;               // Extract ones place

        PlayFile("/temp.wav");
        AnnounceValue(u16temp,tens,ones);
      }
      else if( 0x5A == u8ReceiveBuffer[0]&&
          0x00 == u8ReceiveBuffer[1]&&
          0x00 == u8ReceiveBuffer[2]&&
          0x02 == u8ReceiveBuffer[3]&&
          0x5A == u8ReceiveBuffer[4])
      {
        uint16_t u16temp = (uint16_t)(f32Humid * 10);  // Convert to 1 decimal place
        uint16_t tens = (u16temp / 100) % 10;        // Extract tens place
        uint16_t ones = (u16temp/10) % 10;               // Extract ones place

        PlayFile("/humid.wav");
        AnnounceValue(u16temp,tens,ones);
      }
      else if( 0x5A == u8ReceiveBuffer[0]&&
          0x00 == u8ReceiveBuffer[1]&&
          0x00 == u8ReceiveBuffer[2]&&
          0x03 == u8ReceiveBuffer[3]&&
          0x5A == u8ReceiveBuffer[4])
      {
        PlayFile("/siren.wav");
      }
    }
  }
}


