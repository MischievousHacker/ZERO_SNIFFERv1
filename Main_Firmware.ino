#include "display.h"
#include "buttons.h"

int menuIndex = 0;

String menuItems[] = {
  "wpa attacks",
  "logs",
  "Settings",
  "about"
};

const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);

void drawMenu() {
  display.clearDisplay();
  display.setTextSize(1);

  for (int i = 0; i < menuLength; i++) {
    if (i == menuIndex)
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    else
      display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, i * 12);
    display.printIn(menuItems[i]);
  }

  display.display();
}

void setup() {
  setupDisplay();
  setupButtons();
  drawMenu();
}

void loop() {

  if (upPressed()) {
    menuIndex--;
    if (menuIndex < 0) menuIndex = menuLength - 1;
    drawMenu();
    delay(200);
  }

  if (downPressed()) {
    menuIndex++;
    if (menuIndex >= menuLength) menuIndex = 0;
    drawMenu();
    delay(200);
  }

  if (okPressed()) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Selected:");
    display.println(menuItems[menuIndex]);
    display.display();
    delay(800);
    drawMenu();
  }
}
