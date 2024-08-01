/*H********************************************************************************************
** FILENAME :        buttoncycler.ino
** Version  :        1.0.0
** DESCRIPTION :
**       Activate LED based, and repeats based on arguments
**       green onboard LED signals process-activity
**       RGB_LED signals several state logics based on documentation
** PUBLIC FUNCTIONS :
**      void getLED_Signal(string selection, int repetation, int delayDuration)
**      void getGreenLED_Signal(int delayTime) 
**      void delayLED(int delayTime)
**
** NOTES :
** ToDos :
**
** AUTHOR :            Tjark Ziehm        START DATE :    06 Juli 2024
** Copyright&Licence:  Tjark Ziehm, 2024
** Convention:         <major>.<minor>.<patch>
** CHANGES :
**
*********************************************************************************************H*/


// Simple demonstration on using an input device to trigger changes on your
// NeoPixels. Wire a momentary push button to connect from ground to a
// digital IO pin. When the button is pressed it will change to a new pixel
// animation. Initial state has all pixels off -- press the button once to
// start switching the colors.
// Usage of the onboard green LED @ Pin 27 and the WS2812 RGB-LED @ Pin 25
// Button Pin at the norvi agent one is 35

#include "Wire.h"
#include <string>
#include <Adafruit_NeoPixel.h>
#include <unordered_map>

#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.
#define BUTTON_PIN 35  // Button at the top of norvi agent 1 device
#define PIXEL_PIN 25   // Digital IO pin connected to the NeoPixels for WS2812
#define LED_PIN 27
#define PIXEL_COUNT 1  // Number of NeoPixels

using namespace std;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
//   Argument 1 = Number of pixels in NeoPixel strip
//   Argument 2 = Arduino pin number (most are valid)
//   Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

boolean oldState = HIGH;

std::unordered_map<std::string, int> color = {
  { "red", 1 }, { "yellow", 2 }, { "white", 3 }, { "turquoise", 4 }, { "blue", 5 }, { "green", 6 }, { "off", 7 }, { "led", 8 }
};

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);  // green board led

  strip.begin();  // Initialize NeoPixel strip object (REQUIRED)
  strip.show();   // Initialize all pixels to 'off'
}

void loop() {

  // Get current button state.
  boolean newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if ((newState == LOW) && (oldState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);

    if (newState == LOW) {  // Yes, still low
      //std::string x = "red";
      getLED_Signal("led", 5, 1000);
      //delay(1000);
      //turnOffRGB();
    }
  }

  // Set the last-read button state to the old state.
  oldState = newState;
}


/**
 * Use the green board LED or color RGB LED
 * 
 * @param selection of color options as string: red, green, blue, yellow, turquoise. white, off and led for green onboard led 
 * @param repetation number of blink-signal repetations
 * @param delayDuration duration of time between the signals
 */
void getLED_Signal(string selection, int repetation, int delayDuration) {
  for (int i = 1; i <= repetation; i++) {
    switch (color[selection]) {
      case 1:
        neopixelWrite(PIXEL_PIN, 255, 0, 0);  // red
        delayLED(delayDuration);
        break;
      case 2:
        neopixelWrite(PIXEL_PIN, 255, 255, 0);  // yellow
        delayLED(delayDuration);
        break;
      case 3:
        neopixelWrite(PIXEL_PIN, 255, 255, 255);  // white
        delayLED(delayDuration);
        break;
      case 4:
        neopixelWrite(PIXEL_PIN, 0, 255, 255);  // turquoise
        delayLED(delayDuration);
        break;
      case 5:
        neopixelWrite(PIXEL_PIN, 0, 0, 255);  // blue
        delayLED(delayDuration);
        break;
      case 6:
        neopixelWrite(PIXEL_PIN, 0, 255, 0);  // green
        delayLED(delayDuration);
        break;
      case 7:
        strip.show();  // Initialize all pixels to 'off'
        break;
      case 8:
        getGreenLED_Signal(delayDuration);
        break;
    }
  }
}


void getGreenLED_Signal(int delayTime) {
  digitalWrite(LED_PIN, LOW);  //on
  delay(delayTime);
  digitalWrite(LED_PIN, HIGH);  //off
  delay(delayTime);
  //neopixelWrite(PIXEL_PIN , 0, RGB_BRIGHTNESS, 0);  // Green
}

void delayLED(int delayTime) {
  delay(delayTime);
  strip.show();  // Initialize all pixels to 'off'
  delay(delayTime);
}
