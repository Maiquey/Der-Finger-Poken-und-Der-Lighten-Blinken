#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#define _POSIX_C_SOURCE 200809L
#define NUM_DIRECTIONS 4
#define JSUP_DIRECTION "/sys/class/gpio/gpio26/direction"
#define JSUP_EDGE      "/sys/class/gpio/gpio26/edge"
#define JSUP_IN        "/sys/class/gpio/gpio26/value"
#define JSRT_DIRECTION "/sys/class/gpio/gpio47/direction"
#define JSRT_EDGE      "/sys/class/gpio/gpio47/edge"
#define JSRT_IN        "/sys/class/gpio/gpio47/value"
#define JSDN_DIRECTION "/sys/class/gpio/gpio46/direction"
#define JSDN_EDGE      "/sys/class/gpio/gpio46/edge"
#define JSDN_IN        "/sys/class/gpio/gpio46/value"
#define JSLFT_DIRECTION "/sys/class/gpio/gpio65/direction"
#define JSLFT_EDGE      "/sys/class/gpio/gpio65/edge"
#define JSLFT_IN        "/sys/class/gpio/gpio65/value"

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>		// Errors
#include <string.h>

#include <sys/epoll.h>  // for epoll()
#include <fcntl.h>      // for open()
#include <unistd.h>     // for close()

const char* DirectionFiles[NUM_DIRECTIONS] = {
    JSUP_DIRECTION,
    JSRT_DIRECTION,
    JSDN_DIRECTION,
    JSLFT_DIRECTION
};

const char* ValueFiles[NUM_DIRECTIONS] = {
    JSUP_IN,
    JSRT_IN,
    JSDN_IN,
    JSLFT_IN
};

const char* EdgeFiles[NUM_DIRECTIONS] = {
    JSUP_EDGE,
    JSRT_EDGE,
    JSDN_EDGE,
    JSLFT_EDGE
};

// static void writeToFile(const char* fileName, const char* value);
int waitForGpioEdge(const char** fileNamesForGpioValue);

#endif