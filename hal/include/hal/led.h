#ifndef _LED_H_
#define _LED_H_

#include <stdbool.h>

void led_setAllBrightness(int brightness);
void led_setGameStartingLedState(void);
void led_displayUpLed(bool isUp);

#endif