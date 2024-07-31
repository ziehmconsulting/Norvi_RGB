// Simple demonstration on using an input device to trigger changes on your
// NeoPixels. Wire a momentary push button to connect from ground to a
// digital IO pin. When the button is pressed it will change to a new pixel
// animation. Initial state has all pixels off -- press the button once to
// start the first animation. As written, the button does not interrupt an
// animation in-progress, it works only when idle.
#include "Wire.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.
#define BUTTON_PIN 35  // Button at the top of norvi agent 1 device

#define PIXEL_PIN 25  // Digital IO pin connected to the NeoPixels for WS2812
#define LED_PIN 27

#define PIXEL_COUNT 1  // Number of NeoPixels

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

boolean oldState = HIGH;
uint8_t mode = 0;  // Currently-active animation mode, 0-9
uint8_t pixelActivationDelay = 50;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT); // green board led

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

    if (newState == LOW) {       // Yes, still low
      if (++mode > 5) mode = 0;  // Advance to next mode, wrap around after #4
      switch (mode) {            // Start the new animation...
        case 0:
          wipeColor(strip.Color(0, 0, 0), pixelActivationDelay);  // Black/off
          singleBlinkGreenLED();
          break;
        case 1:
          wipeColor(strip.Color(255, 0, 0), pixelActivationDelay);  // Red
          singleBlinkGreenLED();
          break;
        case 2:
          wipeColor(strip.Color(255, 255, 0), pixelActivationDelay);  // Yellow
          singleBlinkGreenLED();
          break;
        case 3:
          wipeColor(strip.Color(0, 255, 0), pixelActivationDelay);  // Green
          singleBlinkGreenLED();
          break;
        case 4:
          wipeColor(strip.Color(255, 255, 255), pixelActivationDelay);  // White
          singleBlinkGreenLED();
          break;
        case 5:
          wipeColor(strip.Color(0, 0, 255), pixelActivationDelay);  // Blue
          singleBlinkGreenLED();
          break;
      }
    }
  }

  // Set the last-read button state to the old state.
  oldState = newState;
  
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void wipeColor(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
    strip.setPixelColor(i, color);               //  Set pixel's color (in RAM)
    strip.show();                                //  Update strip to match
    delay(wait);                                 //  Pause for a moment
  }
}

void singleBlinkGreenLED() {
  digitalWrite(LED_PIN, LOW); //on
  delay(1000);
  digitalWrite(LED_PIN, HIGH); //off
  //neopixelWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0);  // Green
}
