#ifndef BUTTONS_H
#define BUTTONS_H

#define BTN_OK    D5
#define BTN_DOWN  D6
#define BTN_UP    D7

void setupButtons();
bool upPressed();
bool downPressed();
bool okPressed();

#endif
