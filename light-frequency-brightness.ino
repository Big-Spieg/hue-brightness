/*
  This code is a wrapper for the Adafruit TCS34725 library
  It will print RGB values, color temperature, and lux values to the serial monitor

  Created by: Dorian Spiegel
  Carrier: Ellis-C
*/

#include <Wire.h>
#include "Adafruit_TCS34725.h"

// ------------------ CONSTANTS ------------------

// this is the pin corresponding to the LED on the board
#define led 13

// define the pins for the RGB LED
#define redPin 2
#define greenPin 3
#define bluePin 4

uint16_t trial = 0;
uint16_t numTrials = 200;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

typedef struct {
  int red;
  int green;
  int blue;
} RGB;

// Generate numColors colors that are as different as possible and store them in the rainbow array
void generateRainbow(int numColors, RGB* rainbow) {
  int i;

  for (i = 0; i < numColors; ++i) {
    // Calculate the hue value based on the position in the rainbow
    float hue = i * (360.0 / numColors);

    // Convert hue to RGB values
    rainbow[i].red = 255;
    rainbow[i].green = 0;
    rainbow[i].blue = 0;
    hsvToRgb(&rainbow[i].red, &rainbow[i].green, &rainbow[i].blue, hue);
  }
}

// Convert HSV (hue, saturation, value) to RGB values
void hsvToRgb(int* r, int* g, int* b, float h) {
  int i = static_cast<int>(floor(h / 60.0)) % 6;
  float f = (h / 60.0) - static_cast<int>(floor(h / 60.0));
  float p = 0.0;
  float q = 1.0 - f;
  float t = f;

  switch (i) {
    case 0:
      *r = 255;
      *g = floor(255 * t);
      *b = floor(255 * p);
      break;
    case 1:
      *r = floor(255 * q);
      *g = 255;
      *b = floor(255 * p);
      break;
    case 2:
      *r = floor(255 * p);
      *g = 255;
      *b = floor(255 * t);
      break;
    case 3:
      *r = floor(255 * p);
      *g = floor(255 * q);
      *b = 255;
      break;
    case 4:
      *r = floor(255 * t);
      *g = floor(255 * p);
      *b = 255;
      break;
    case 5:
      *r = 255;
      *g = floor(255 * p);
      *b = floor(255 * q);
      break;
  }
}

// define rainbow
RGB* rainbow;

void setup() {
  Serial.println("Starting up ...");
  rainbow = (RGB*)malloc(sizeof(RGB) * 10);
  generateRainbow(numTrials, rainbow);

  pinMode(led, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // turn onboard led off
  digitalWrite(led, HIGH);

  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No sensor found ... check your connections");
    while (1); // stop here *halt
  }
}

void loop() {
  uint16_t tests = 3;

  for (int i = 0; i < tests; i++) {
    if (trial > numTrials - 1) {
      Serial.println("\n\n\n\n\n\n ------- NEW DATA ------- \n\n\n\n\n\n");
      trial = 0;
    }

    char dataOut[50];
    sprintf(dataOut, "%d,", trial+1);

    RGB color = rainbow[trial];
    analogWrite(redPin, color.red);
    analogWrite(greenPin, color.green);
    analogWrite(bluePin, color.blue);

    sprintf(dataOut + strlen(dataOut), "%d,%d,%d,", color.red, color.green, color.blue);

    uint16_t r, g, b, c, colorTemp, lux;

    delay(75);

    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature(r, g, b);
    lux = tcs.calculateLux(r, g, b);

    // takes 50 ms for the sensor to update

    sprintf(dataOut + strlen(dataOut), "%d,%d,%d,%d,%d,%d", colorTemp, lux, r, g, b, c);
    Serial.println(dataOut);
  }

  trial += 1;
}


