#ifndef RGBLED_H
#define RGBLED_H

#include <Arduino.h>

#define R 0
#define G 1
#define B 2
#define RGB 3

#define LED_OFF 0
#define WHITE 1
#define RED 2
#define YELLOW 3
#define GREEN 4
#define CYAN 5
#define BLUE 6
#define MAGENTA 7

#define TOTAL_COLORS 8

typedef float RGBValue[RGB];

RGBValue rgbLedColors[TOTAL_COLORS] = {
  { 0, 0, 0 },        // LED_OFF
  { 255, 255, 255 },  // WHITE
  { 255, 0, 0 },      // RED
  { 255, 255, 0 },    // YELLOW
  { 0, 255, 0 },      // GREEN
  { 0, 255, 255 },    // CYAN
  { 0, 0, 255 },      // BLUE
  { 255, 0, 255 },    // MAGENTA
};

struct RGBLedFade {
  uint8_t stepCount;
  uint16_t durationMs;
  uint16_t stepIntervalMs;  // durationMs / stepCount;
  uint8_t stepIndex = 0;
  float rgbDelta[RGB] = { 0 };

  bool isCycle;
  int8_t cyclePhase = 0;
  uint32_t stepStartTime = 0;
  uint8_t stepNumber = 0;
  uint8_t startColor;
  uint8_t endColor;

  RGBLedFade(
    uint8_t startColor = 0,
    uint8_t endColor = 1,
    bool isCycle = false,
    uint16_t durationMs = 2000,
    uint8_t stepCount = 100)
    : startColor(startColor),
      endColor(endColor),
      isCycle(isCycle),
      durationMs(durationMs),
      stepCount(stepCount) {
    stepIntervalMs = durationMs / stepCount;
  }

  bool isStepComplete(uint32_t now) {
    return now - stepStartTime >= stepIntervalMs;
  }

  bool isFadeComplete() {
    return stepNumber == stepCount;
  }

  void setFadeDelta(RGBValue colors[TOTAL_COLORS]) {
    // TODO: throw or return error code if no start and end color
    for (uint8_t i = 0; i < RGB; i++) {
      rgbDelta[i] = (colors[endColor][i] - colors[startColor][i]) / (float)stepCount;
    }
  }
};

class RGBLed {
public:

  uint8_t pins[3];

  RGBLed(uint8_t rPin, uint8_t gPin, uint8_t bPin) {
    pins[R] = rPin;
    pins[G] = gPin;
    pins[B] = bPin;
    for (uint8_t pin = 0; pin < RGB; pin++) {
      pinMode(pins[pin], OUTPUT);
    }
  }

  float rgb[RGB] = { 0 };

  void emit(uint8_t color) {
    clearFade();
    setRgb(rgbLedColors[color]);
    for (uint8_t i = 0; i < RGB; i++) {
      digitalWrite(pins[i], rgbLedColors[color][i]);
    }
  }

  // TODO: needed?
  // void emitAnalog(uint8_t color) {
  //   setRgb(colors[color]);
  //   analogWriteToPins(rgb);
  // }

  void emitCustomColor(float rgbValue[RGB]) {
    setRgb(rgbValue);
    analogWriteToPins(rgb);
  }

  RGBLedFade* fade = nullptr;

  /** 
  To be used with `updateFade()`. Configures the fade that `updateFade()` 
  executes over the frames / steps of the consuming program.
  NOTE: Currently, the fading only works with the predefined `colors`.
  */
  // void setFade(uint8_t startColor, uint8_t endColor, uint16_t durationMs, bool isCycle) {
  void setFade(RGBLedFade* f) {
    fade = f;
    setRgb(rgbLedColors[fade->startColor]);
    fade->setFadeDelta(rgbLedColors);
  }

  void clearFade() {
    fade = nullptr;
  }

  void updateFade() {
    if (!fade) return;
    const uint32_t now = millis();
    if (!fade->isStepComplete(now)) return;
    // completed a step
    if (fade->isCycle && fade->isFadeComplete()) {
      // this step completes a fade (or a 1/2 cycle, if the fade is set to cycle)
      // reverse the fade
      uint8_t tmp = fade->startColor;
      fade->startColor = fade->endColor;
      fade->endColor = tmp;
      setRgb(rgbLedColors[fade->startColor]);
      fade->setFadeDelta(rgbLedColors);
      fade->cyclePhase = !fade->cyclePhase;
      fade->stepNumber = 0;
    } else {
      fade->stepNumber++;
    }
    fade->stepStartTime = now;
    update();
  }

private:

  /**
  Update the R, G, & B values of `currentRGB` and `analogWrite`
  the updated value to the LED pins.
  */
  void update() {
    for (uint8_t i = 0; i < RGB; i++) rgb[i] += fade->rgbDelta[i];
    analogWriteToPins(rgb);
  }

  void setRgb(float rgbValue[RGB]) {
    for (uint8_t i = 0; i < RGB; i++) rgb[i] = rgbValue[i];
  }

  /**
  Writes a floating point set of RGB values to analog outputs (`pins`) as rounded ints.
  */
  void analogWriteToPins(RGBValue rgbValue) {
    for (uint8_t i = 0; i < RGB; i++) analogWriteRoundedFloat(pins[i], rgbValue[i]);
  }

  void analogWriteRoundedFloat(uint8_t pin, float value) {
    // TODO: error if value out of bounds of uint8
    analogWrite(pin, lround(value));
  }
};

#endif  // RGBLED_H
