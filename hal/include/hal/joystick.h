#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>		// Errors
#include <string.h>

#include <sys/epoll.h>  // for epoll()
#include <fcntl.h>      // for open()
#include <unistd.h>     // for close()



// static int readLineFromFile(const char* fileName, char* buff, unsigned int maxLength);
// static void writeToFile(const char* fileName, const char* value);
// static int waitForGpioEdge(const char** fileNamesForGpioValue);
void joystick_init(void);
int joystick_getJoyStickPress(void);

#endif