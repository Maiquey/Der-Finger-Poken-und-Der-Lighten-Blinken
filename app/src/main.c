// Main program to build the application
// Has main(); does initialization and cleanup and perhaps some basic logic.
// Houses main logic loop of the Der-Finger-Poken & Der-Lighten-Blinken Game

#define _POSIX_C_SOURCE 200809L
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "timing.h"
#include "hal/led.h"
#include "hal/joystick.h"

#define NUM_LEDS 4
#define LED_UP 0
#define LED_DOWN 3
#define CORRECT_FREQUENCY 4
#define INCORRECT_FREQUENCY 10

#define JOYSTICK_UP 0
#define JOYSTICK_RIGHT 1
#define JOYSTICK_DOWN 2
#define JOYSTICK_LEFT 3

#define TIMEOUT_CODE 4
#define ERROR_CODE -1

#define MAX_TIMEOUT 5000
#define MIN_WAIT_TIME 500
#define MAX_WAIT_TIME 3000

//Flash LEDs at 4Hz for 0.5 seconds
static void flashCorrect(void){
    for (int i = 0; i < CORRECT_FREQUENCY; i++){
        led_setAllBrightness(i % 2);
        sleepForMs(125);
    }
}

//Flash LEDs at 10Hz for 1 second
static void flashIncorrect(void){
    for (int i = 0; i < INCORRECT_FREQUENCY; i++){
        led_setAllBrightness(i % 2);
        sleepForMs(50);
    }
}

int main()
{
    srand(time(NULL));
    joystick_init(); //sets GPIO, input, and edge triggers

    printf("Welcome to Der-Finger-Poken & Der-Lighten-Blinken!\n\n");
    printf("When the LEDs light up, press the joystick in that direction!\n");
    printf("(Press left or right to exit)\n");
    long long bestTime = MAX_TIMEOUT; // Will never go above this value
    
    while(1) {
        bool loopBack = false; // bool value for pre-emptive joystick trigger ("Too Soon" handling)
        led_setGameStartingLedState();
        printf("Get ready...\n");

        if (joystick_checkIfPressed()){
            printf("Please let go of joystick\n");
            joystick_waitForRelease();
        }
        
        long long waitTime = (rand() % (MAX_WAIT_TIME - MIN_WAIT_TIME + 1) + MIN_WAIT_TIME); // Courtesy of chatGPT
        printf("Wait time is %lldms\n", waitTime);
        long long startWaitTime = getTimeInMs();

        // loop for specified waitTime
        while (getTimeInMs() - startWaitTime < waitTime){
            // Ignore left and right presses on the joystick
            if (joystick_isPressedUpDown()){ //break the waiting time cycle if pre-emptive press is triggered ("Too Soon" handling)
                loopBack = true;
                break;
            }
        }
        if (loopBack) {
            printf("Too soon!\n");
            // Incorrect flash to provide visual indicator that the user is doing something wrong
            // Also allows the user a second of time to release the joystick before the game loop starts at step 1 again
            flashIncorrect();
            continue;
        }

        // If user is still holding the joystick left or right after wait time finishes, exit program
        if (joystick_isPressedLeftRight()){
            printf("User selected to quit.\n");
            led_setAllBrightness(0);
            exit(EXIT_SUCCESS);
        }

        // Wait time successfully concluded, proceed to reaction game

        led_setAllBrightness(0); // prepare LEDs for displaying up or down
        int gameDirection = (rand() % 2) * LED_DOWN; //returns 0 or 3 randomly (LED_UP or LED_DOWN) - Courtesy of chatGPT
        if (gameDirection == LED_UP){
            printf("Press UP now!\n");
            led_displayUpLed(true);
        } else {
            printf("Press DOWN now!\n");
            led_displayUpLed(false);
        }

        long long startTime = getTimeInMs();
        int directionId = joystick_getJoyStickPress(MAX_TIMEOUT); // Get direction input from user
        long long endTime = getTimeInMs();
        
        long long timeTaken = endTime - startTime;

        // Process output from joystick edge trigger function
        if (directionId == TIMEOUT_CODE) {
            printf("No input within 5000ms; quitting!\n");
            led_setAllBrightness(0);
            exit(EXIT_SUCCESS);
        } else if (directionId == JOYSTICK_LEFT || directionId == JOYSTICK_RIGHT){
            printf("User selected to quit.\n");
            led_setAllBrightness(0);
            exit(EXIT_SUCCESS);
        } else if ((directionId == JOYSTICK_UP && gameDirection == LED_UP)
                || (directionId == JOYSTICK_DOWN && gameDirection == LED_DOWN)) {
            printf("Correct!\n");
            if (timeTaken < bestTime){
                bestTime = timeTaken;
                printf("New best time!\n");
            }
            printf("Your reaction time was %lldms; best so far in game is %lldms.\n", timeTaken, bestTime);
            flashCorrect();
        } else {
            printf("Incorrect.\n");
            flashIncorrect();
        }
    }

}