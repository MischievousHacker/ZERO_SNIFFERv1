#include <Arduino.h>
#include "buttons.h"

void setupButtons() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
}

bool upPressed() {
  return digitalRead(BTN_UP) == LOW;
}

bool downPressed() {
  return digitalRead(BTN_DOWN) == LOW;
}

bool okPressed() {
  return digitalRead(BTN_OK) == LOW;
}
