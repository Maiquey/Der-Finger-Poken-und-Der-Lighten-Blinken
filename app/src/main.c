// Main program to build the application
// Has main(); does initialization and cleanup and perhaps some basic logic.

#define _POSIX_C_SOURCE 200809L
#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>
#include "badmath.h"
#include "timing.h"
#include "hal/button.h"
#include "hal/led.h"

#define NUM_LEDS 4

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
static void setAllBrightness(int brightness){
    for (int i = 0; i < NUM_LEDS; i++){
        led_setBrightness(i, brightness);
    }
}

int main()
{
    printf("Hello world!\n");

    // Initialize all modules; HAL modules first
    button_init();
    badmath_init();

    // Main program logic:
    // for (int i = 0; i < 10; i++) {
    //     printf("  -> Reading button time %d = %d\n", i, button_is_button_pressed());
    // }

    //INITIAL LED TESTING
    // long long startTime = getTimeInMs();
    // led_setAllToNone();
    // sleepForMs(5000);
    // led_setAllToTimer();
    // sleepForMs(5000);
    // led_setAllToNone();
    // long long endTime = getTimeInMs();

    // printf("I slept for a total of %lld milliseconds\n", endTime - startTime);

    //ADDITIONAL LED TESTING
    led_setAllToNone();
    led_setBrightness(0, 1);
    sleepForMs(500);
    led_setBrightness(1, 1);
    sleepForMs(500);
    led_setBrightness(2, 1);
    sleepForMs(500);
    led_setBrightness(3, 1);
    sleepForMs(2000);
    led_setBrightness(3, 0);
    sleepForMs(500);
    led_setBrightness(2, 0);
    sleepForMs(500);
    led_setBrightness(1, 0);
    sleepForMs(500);
    led_setBrightness(0, 0);
    sleepForMs(500);
    setAllBrightness(1);
    sleepForMs(100);
    setAllBrightness(0);
    sleepForMs(100);
    setAllBrightness(1);
    sleepForMs(100);
    setAllBrightness(0);

    // for (int i = 0; i <= 35; i++) {
    //     int ans = badmath_factorial(i);
    //     printf("%4d! = %6d\n", i, ans);
    // }

    // printf("%lld milliseconds elapsed since epoch\n", getTimeInMs());
    // Cleanup all modules (HAL modules last)
    badmath_cleanup();
    button_cleanup();

    printf("!!! DONE !!!\n"); 

    // Some bad code to try out and see what shows up.
    #if 0
        // Test your linting setup
        //   - You should see a warning underline in VS Code
        //   - You should see compile-time errors when building (-Wall -Werror)
        // (Linting using clang-tidy; see )
        int x = 0;
        if (x = 10) {
        }
    #endif
    #if 0
        // Demonstrate -fsanitize=address (enabled in the root CMakeFiles.txt)
        // Compile and run this code. Should see warning at compile time; error at runtime.
        int data[3];
        data[3] = 10;
        printf("Value: %d\n", data[3]);
    #endif
}