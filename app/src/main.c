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
#define JOYSTICK_UP 0
#define JOYSTICK_RIGHT 1
#define JOYSTICK_DOWN 2
#define JOYSTICK_LEFT 3
#define LED_UP 0
#define LED_DOWN 3
#define UP_DIRECTION 0
#define TIMEOUT_CODE 4
#define CORRECT_FREQUENCY 4
#define INCORRECT_FREQUENCY 10
#define MAX_TIMEOUT 5000

// From Assignment Description
// static void runCommand(char* command)
// {
//     // Execute the shell command (output into pipe)
//     FILE *pipe = popen(command, "r");

//     // Ignore output of the command; but consume it
//     // so we don't get an error when closing the pipe.
//     char buffer[1024];
//     while (!feof(pipe) && !ferror(pipe)) {
//         if (fgets(buffer, sizeof(buffer), pipe) == NULL)
//             break;
//         // printf("--> %s", buffer); // Uncomment for debugging
//     }

//     // Get the exit code from the pipe; non-zero is an error:
//     int exitCode = WEXITSTATUS(pclose(pipe));
//     if (exitCode != 0) {
//         perror("Unable to execute command:");
//         printf(" command: %s\n", command);
//         printf(" exit code: %d\n", exitCode);
//     }
// }
static void flashCorrect(void){
    for (int i = 0; i < CORRECT_FREQUENCY; i++){
        led_setAllBrightness(i % 2);
        sleepForMs(125);
    }
}

static void flashIncorrect(void){
    for (int i = 0; i < INCORRECT_FREQUENCY; i++){
        led_setAllBrightness(i % 2);
        sleepForMs(50);
    }
}

int main()
{

    // printf("%lld milliseconds elapsed since epoch\n", getTimeInMs());
    // Cleanup all modules (HAL modules last)
    // badmath_cleanup();
    // button_cleanup();

    // REAL CODE
    // TODO: export GPIO pins
    srand(time(NULL));
    joystick_init();

    printf("Welcome to the reaction timer game v1.0!\n");
    printf("When the LEDs light up, press the joystick in that direction!\n");
    printf("(Press left or right to exit)\n");
    long long bestTime = 5000;
    
    while(1) {
        bool loopBack = false;
        led_setGameStartingLedState();
        printf("Get ready...\n");

        if (joystick_checkIfPressed()){
            printf("Please let go of joystick\n");
            joystick_waitForRelease();
        }
        //Thank you chatGPT
        long long waitTime = (rand() % (3000 - 500 + 1) + 500);
        long long startWaitTime = getTimeInMs();
        while (getTimeInMs() - startWaitTime < waitTime){
            if (joystick_isPressedUpDown()){
                loopBack = true;
                break;
            }
        }
        if (loopBack) {
            printf("Too soon!\n");
            flashIncorrect();
            continue;
        }
        if (joystick_isPressedLeftRight()){
            printf("User selected to quit.\n");
            led_setAllBrightness(0);
            exit(EXIT_SUCCESS);
        }
        // int directionId = joystick_getJoyStickPress(waitTime);
        // sleepForMs(waitTime);
        // printf("Direction: %d\n", directionId);
        // if (directionId == JOYSTICK_UP || directionId == JOYSTICK_DOWN) {
        //     printf("Too soon!\n");
        //     flashIncorrect();
        //     continue;
        // }

        led_setAllBrightness(0);
        int direction = (rand() % 2) * 3; //returns 0 or 3 randomly (for up or down LED)
        if (direction == LED_UP){
            printf("Press UP now!\n");
            led_displayUpLed(true);
        } else {
            printf("Press DOWN now!\n");
            led_displayUpLed(false);
        }

        long long startTime = getTimeInMs();
        int directionId = joystick_getJoyStickPress(MAX_TIMEOUT);
        long long endTime = getTimeInMs();
        // printf("Direction: %d\n", directionId);
        
        long long timeTaken = endTime - startTime;

        if (directionId == TIMEOUT_CODE) {
            printf("No input within 5000ms; quitting!\n");
            led_setAllBrightness(0);
            exit(EXIT_SUCCESS);
        } else if (directionId == JOYSTICK_LEFT || directionId == JOYSTICK_RIGHT){
            printf("User selected to quit.\n");
            led_setAllBrightness(0);
            exit(EXIT_SUCCESS);
        } else if ((directionId == JOYSTICK_UP && direction == UP_DIRECTION)
                || (directionId == JOYSTICK_DOWN && direction != UP_DIRECTION)) {
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

    // REAL CODE END

    printf("!!! DONE !!!\n"); 
}