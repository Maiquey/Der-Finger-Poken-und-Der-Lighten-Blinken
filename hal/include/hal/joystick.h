#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h>		// Errors
#include <string.h>

#include <sys/epoll.h>  // for epoll()
#include <fcntl.h>      // for open()
#include <unistd.h>     // for close()



// static int readLineFromFile(const char* fileName, char* buff, unsigned int maxLength);
// static void writeToFile(const char* fileName, const char* value);
// static int waitForGpioEdge(const char** fileNamesForGpioValue);
void joystick_init(void);
int joystick_getJoyStickPress(long long timeout);
bool joystick_checkIfPressed(void);
void joystick_waitForRelease(void);
bool joystick_isPressedUpDown(void);
bool joystick_isPressedLeftRight(void);

#endif