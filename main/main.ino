// pin to use to flash lights
#define VOLUME 1
#include <zyButton.h>

#include <AlarmLED.h>

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

// create shield-example object!
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

AlarmLED led(5, 200);

zyButton button_fire(2);
zyButton button_effect_1(3);
zyButton button_effect_2(4);
zyButton button_effect_3(5);
zyButton button_effect_4(6);
zyButton button_effect_5(7);

void setup() {
  Serial.begin(9600);
  
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }

   Serial.println(F("VS1053 found"));
   
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(VOLUME,VOLUME);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  button_fire.init(musicPlayer);
  button_effect_1.init(musicPlayer);
  button_effect_2.init(musicPlayer);
  button_effect_3.init(musicPlayer);
  button_effect_4.init(musicPlayer);
  button_effect_5.init(musicPlayer);
}

bool on = false;

void loop() {
  button_fire.loop();
  button_effect_1.loop();
  button_effect_2.loop();
  button_effect_3.loop();
  button_effect_4.loop();
  button_effect_5.loop();
  
  if(button_fire.isDown() && on == false) {
    led.start();
    Serial.println("Start Alarm"); 
    musicPlayer.startPlayingFile("/track001.ogg");
    on = true;
  } else {
    if(button_fire.isUp() && on == true) {
      led.stop();
      Serial.println("Stop Alarm");
      musicPlayer.stopPlaying();
      on = false;
    }

    if(musicPlayer.playingMusic && (
      button_effect_1.isPressed() ||
      button_effect_2.isPressed() ||
      button_effect_3.isPressed() ||
      button_effect_4.isPressed() ||
      button_effect_5.isPressed())
    ) {
      musicPlayer.stopPlaying();
    } else {
      if(button_effect_1.isPressed()) {
        Serial.println("Effect 1");
        musicPlayer.startPlayingFile("/effect01.mp3");
      } else if(button_effect_2.isPressed()) {
        Serial.println("Effect 2");
        musicPlayer.startPlayingFile("/effect02.mp3");
      } else if(button_effect_3.isPressed()) {
        Serial.println("Effect 3");
        musicPlayer.startPlayingFile("/effect03.mp3");
      } else if(button_effect_4.isPressed()) {
        Serial.println("Effect 4");
        musicPlayer.startPlayingFile("/effect04.mp3");
      } else if(button_effect_5.isPressed()) {
        Serial.println("Effect 5");
        musicPlayer.startPlayingFile("/effect05.mp3");
      }
    }
  }

  led.loop();
}
