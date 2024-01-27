#include "hal/led.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LED_0_TRIGGER_FILE "/sys/class/leds/beaglebone:green:usr0/trigger"
#define LED_1_TRIGGER_FILE "/sys/class/leds/beaglebone:green:usr1/trigger"
#define LED_2_TRIGGER_FILE "/sys/class/leds/beaglebone:green:usr2/trigger"
#define LED_3_TRIGGER_FILE "/sys/class/leds/beaglebone:green:usr3/trigger"
#define LED_0_BRIGHTNESS_FILE "/sys/class/leds/beaglebone:green:usr0/brightness"
#define LED_1_BRIGHTNESS_FILE "/sys/class/leds/beaglebone:green:usr1/brightness"
#define LED_2_BRIGHTNESS_FILE "/sys/class/leds/beaglebone:green:usr2/brightness"
#define LED_3_BRIGHTNESS_FILE "/sys/class/leds/beaglebone:green:usr3/brightness"

// #define LED_0_DELAY_ON "/sys/class/leds/beaglebone:green:usr0/delay_on"
// #define LED_1_DELAY_ON "/sys/class/leds/beaglebone:green:usr1/delay_on"
// #define LED_2_DELAY_ON "/sys/class/leds/beaglebone:green:usr2/delay_on"
// #define LED_3_DELAY_ON "/sys/class/leds/beaglebone:green:usr3/delay_on"
// #define LED_0_DELAY_OFF "/sys/class/leds/beaglebone:green:usr0/delay_off"
// #define LED_1_DELAY_OFF "/sys/class/leds/beaglebone:green:usr1/delay_off"
// #define LED_2_DELAY_OFF "/sys/class/leds/beaglebone:green:usr2/delay_off"
// #define LED_3_DELAY_OFF "/sys/class/leds/beaglebone:green:usr3/delay_off"

#define NUM_LEDS 4
#define LED_UP 0
#define LED_DOWN 3

static char* led_triggers[NUM_LEDS] = {
    LED_0_TRIGGER_FILE,
    LED_1_TRIGGER_FILE,
    LED_2_TRIGGER_FILE,
    LED_3_TRIGGER_FILE
};

static char* led_brightness[NUM_LEDS] = {
    LED_0_BRIGHTNESS_FILE,
    LED_1_BRIGHTNESS_FILE,
    LED_2_BRIGHTNESS_FILE,
    LED_3_BRIGHTNESS_FILE
};

// static char* led_delay_on[NUM_LEDS] = {
//     LED_0_DELAY_ON,
//     LED_1_DELAY_ON,
//     LED_2_DELAY_ON,
//     LED_3_DELAY_ON
// };

// static char* led_delay_off[NUM_LEDS] = {
//     LED_0_DELAY_OFF,
//     LED_1_DELAY_OFF,
//     LED_2_DELAY_OFF,
//     LED_3_DELAY_OFF
// };

static void setSingleLedToNone(int ledId){
    char* triggerFile = led_triggers[ledId];
    FILE *pLedTriggerFile = fopen(triggerFile, "w");

    if (pLedTriggerFile == NULL){
        printf("ERROR OPENING %s.", triggerFile);
        exit(1);
    }

    int charWritten = fprintf(pLedTriggerFile, "none");
    if (charWritten <= 0) {
        printf("ERROR WRITING DATA");
        exit(1);
    }

    fclose(pLedTriggerFile);
}

// static void setSingleLedToTimer(int ledId){
//     char* triggerFile = led_triggers[ledId];
//     FILE *pLedTriggerFile = fopen(triggerFile, "w");

//     if (pLedTriggerFile == NULL){
//         printf("ERROR OPENING %s.", triggerFile);
//         exit(1);
//     }

//     int charWritten = fprintf(pLedTriggerFile, "timer");
//     if (charWritten <= 0) {
//         printf("ERROR WRITING DATA");
//         exit(1);
//     }

//     fclose(pLedTriggerFile);
// }

// static void setLedDelayOn(int ledId, const char* delayString){
//     char* delayFile = led_delay_on[ledId];
//     FILE *pLedDelayFile = fopen(delayFile, "w");

//     if (pLedDelayFile == NULL){
//         printf("ERROR OPENING %s.", delayFile);
//         exit(1);
//     }

//     char* delayStr = "hi";
//     int charWritten = fprintf(pLedDelayFile, delayStr);
//     if (charWritten <= 0) {
//         printf("ERROR WRITING DATA");
//         exit(1);
//     }

//     fclose(pLedDelayFile);
// }

static void setAllToNone(void){
    for (int i = 0; i < NUM_LEDS; i++){
        setSingleLedToNone(i);
    }
}

// static void setAllToTimer(void){
//     for (int i = 0; i < NUM_LEDS; i++){
//         setSingleLedToTimer(i);
//     }
// }

//TODO change to int input
static void setBrightnessOneLed(int ledId, int brightness) {
    char* brightnessFile = led_brightness[ledId];
    FILE *pLedBrightnessFile = fopen(brightnessFile, "w");

    if (pLedBrightnessFile == NULL){
        printf("ERROR OPENING %s.", brightnessFile);
        exit(1);
    }

    int charWritten;
    if (brightness == 1){
        charWritten = fprintf(pLedBrightnessFile, "1");
    } else {
        charWritten = fprintf(pLedBrightnessFile, "0");
    }
    
    if (charWritten <= 0) {
        printf("ERROR WRITING DATA");
        exit(1);
    }

    fclose(pLedBrightnessFile);
}

void led_setAllBrightness(int brightness){
    setAllToNone();
    for (int i = 0; i < NUM_LEDS; i++){
        setBrightnessOneLed(i, brightness);
    }
}

void led_setGameStartingLedState(void){
    setAllToNone();
    for (int i = 0; i < NUM_LEDS; i++){
        if (i == 0 || i == 3) {
            setBrightnessOneLed(i, 0);
        } else {
            setBrightnessOneLed(i, 1);
        }
    }
}

void led_displayUpLed(bool isUp){
    if (isUp){
        setBrightnessOneLed(LED_UP, 1);
    } else {
        setBrightnessOneLed(LED_DOWN, 1);
    }
}

// void led_correctFlash(void){
//     led_setAllBrightness(0);
//     setAllToTimer();
//     //4Hz = 4 times a second (1000ms)
//     //so 4x125ms of on and 4x125ms of off
//     for (int i = 0; i < NUM_LEDS; i++){
//         //set delay_on
//         char* delayFile = led_delay_on[i];
//         FILE *pLedDelayFile = fopen(delayFile, "w");

//         if (pLedDelayFile == NULL){
//             printf("ERROR OPENING %s.", delayFile);
//             exit(1);
//         }

//         int charWritten = fprintf(pLedDelayFile, "125");
//         if (charWritten <= 0) {
//             printf("ERROR WRITING DATA");
//             exit(1);
//         }

//         fclose(pLedDelayFile);

//         //set delay_off
//         delayFile = led_delay_off[i];
//         pLedDelayFile = fopen(delayFile, "w");

//         if (pLedDelayFile == NULL){
//             printf("ERROR OPENING %s.", delayFile);
//             exit(1);
//         }

//         charWritten = fprintf(pLedDelayFile, "125");
//         if (charWritten <= 0) {
//             printf("ERROR WRITING DATA");
//             exit(1);
//         }

//         fclose(pLedDelayFile);
//     }
// }

// void led_incorrectFlash(void){
//     return;
// }