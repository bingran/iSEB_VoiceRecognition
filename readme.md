
# ESP32 Voice-Activated Temperature and Humidity Monitor

## Overview

This project uses an ESP32-S3 with an OLED display, temperature and humidity sensor, and I2S audio output. It recognizes voice commands and responds with spoken temperature and humidity readings using WAV audio files.

## Features

-   **Voice Recognition**: Responds to custom wake words and commands.
    
-   **OLED Display**: Shows temperature and humidity data.
    
-   **Audio Playback**: Uses I2S output to play WAV files for responses.
    
-   **UART Communication**: Interfaces with VC-02 voice module.
    
-   **LittleFS Support**: Stores and plays pre-recorded audio files.
    

## Hardware Requirements

-   ESP32-S3
    
-   VC-02 voice recognition module
    
-   SHT31 temperature & humidity sensor
    
-   SSD1306 OLED display (128x64)
    
-   MAX98357A I2S audio amplifier
    
-   Speaker
    

## Software Requirements

-   Arduino IDE
    
-   ESP32 board support package
    
-   Libraries:
    
    -   Adafruit GFX
        
    -   Adafruit SSD1306
        
    -   Adafruit SHT31
        
    -   LittleFS
        
    -   Audio libraries
        

## Installation & Setup

1.  **Install Arduino IDE** and add ESP32 board support.
    
2.  **Install required libraries** via Library Manager.
    
3.  **Upload code** to ESP32.
    
4.  **Upload WAV files** to ESP32's LittleFS storage.
    
5.  **Connect hardware components** as per the defined pin connections.
    

## Commands & Responses

-   **"Hello"** → Plays greeting audio.
    
-   **"What is the temperature?"** → Reads and announces the temperature.
    
-   **"What is the humidity?"** → Reads and announces humidity.
    
-   **"Play siren"** → Plays a siren sound.
    

## File Structure

-   `/hi.wav` – Greeting sound
    
-   `/temp.wav` – "Temperature is" announcement
    
-   `/humid.wav` – "Humidity is" announcement
    
-   `/siren.wav` – Siren sound
    
-   `/*.wav` – Number audio files (0-9, 10, 20, etc.)
    

## Troubleshooting

-   **No sound output?**
    
    -   Check I2S pin connections.
        
    -   Verify audio files are uploaded to LittleFS.
        
-   **Incorrect temperature readings?**
    
    -   Ensure SHT31 sensor is connected properly.
        
-   **No response to voice commands?**
    
    -   Check VC-02 UART communication.
        

## Future Enhancements

-   Support for additional voice commands.
    
-   Improved TTS using ESP32 software TTS.
    
-   Dynamic WAV file generation.
    

## License

This project is open-source under the MIT License.# ESP32 Voice-Activated Temperature and Humidity Monitor

## Overview

This project uses an ESP32-S3 with an OLED display, temperature and humidity sensor, and I2S audio output. It recognizes voice commands and responds with spoken temperature and humidity readings using WAV audio files.

## Features

-   **Voice Recognition**: Responds to custom wake words and commands.
    
-   **OLED Display**: Shows temperature and humidity data.
    
-   **Audio Playback**: Uses I2S output to play WAV files for responses.
    
-   **UART Communication**: Interfaces with VC-02 voice module.
    
-   **LittleFS Support**: Stores and plays pre-recorded audio files.
    

## Hardware Requirements

-   ESP32-S3
    
-   VC-02 voice recognition module
    
-   SHT31 temperature & humidity sensor
    
-   SSD1306 OLED display (128x64)
    
-   MAX98357A I2S audio amplifier
    
-   Speaker
    

## Software Requirements

-   Arduino IDE
    
-   ESP32 board support package
    
-   Libraries:
    
    -   Adafruit GFX
        
    -   Adafruit SSD1306
        
    -   Adafruit SHT31
        
    -   LittleFS
        
    -   Audio libraries
        

## Installation & Setup

1.  **Install Arduino IDE** and add ESP32 board support.
    
2.  **Install required libraries** via Library Manager.
    
3.  **Upload code** to ESP32.
    
4.  **Upload WAV files** to ESP32's LittleFS storage.
    
5.  **Connect hardware components** as per the defined pin connections.
    

## Commands & Responses

-   **"Hello"** → Plays greeting audio.
    
-   **"What is the temperature?"** → Reads and announces the temperature.
    
-   **"What is the humidity?"** → Reads and announces humidity.
    
-   **"Play siren"** → Plays a siren sound.
    

## File Structure

-   `/hi.wav` – Greeting sound
    
-   `/temp.wav` – "Temperature is" announcement
    
-   `/humid.wav` – "Humidity is" announcement
    
-   `/siren.wav` – Siren sound
    
-   `/*.wav` – Number audio files (0-9, 10, 20, etc.)
    

## Troubleshooting

-   **No sound output?**
    
    -   Check I2S pin connections.
        
    -   Verify audio files are uploaded to LittleFS.
        
-   **Incorrect temperature readings?**
    
    -   Ensure SHT31 sensor is connected properly.
        
-   **No response to voice commands?**
    
    -   Check VC-02 UART communication.
        

## Future Enhancements

-   Support for additional voice commands.
    
-   Improved TTS using ESP32 software TTS.
    
-   Dynamic WAV file generation.
    

