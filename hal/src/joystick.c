#include "hal/joystick.h"

int waitForGpioEdge(const char** fileNamesForGpioValue) 
{
    int fdList[NUM_DIRECTIONS];
	// If you want to wait for input on multiple file, you could change this function
	// to take an array of names, and then loop throug each, adding it to the 
	// epoll instance (accessed via epollfd) by calling epoll_ctl() numerous times.

	// create an epoll instance
	// .. kernel ignores size argument; must be > 0

    //TODO
    // Make array of names, and add each to epoll instance (epollfd) by calling epoll_ctl()

	int epollfd = epoll_create(1);
	if (epollfd == -1) {
		fprintf(stderr, "ERROR: epoll_create() returned (%d) = %s\n", epollfd, strerror(errno));
		return -1;
	}

	struct epoll_event events[NUM_DIRECTIONS];

	// open GPIO value file:
    for (int i = 0; i < NUM_DIRECTIONS; i++){
		int fdLeft = open(fileNamesForGpioValue[i], O_RDONLY | O_NONBLOCK);
		fdList[i] = fdLeft;
		if (fdLeft == -1) {
			fprintf(stderr, "ERROR: unable to open() GPIO value file (%d) = %s\n", fdLeft, strerror(errno));
			return -1;
		}

		// configure epoll to wait for events: read operation | edge triggered | urgent data
		struct epoll_event epollStruct;
		epollStruct.events = EPOLLIN | EPOLLET | EPOLLPRI;
		epollStruct.data.fd = fdLeft;

		// register file descriptor on the epoll instance, see: man epoll_ctl
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fdLeft, &epollStruct) == -1) {
			fprintf(stderr, "ERROR: epoll_ctl() failed on add control interface (%d) = %s\n", fdLeft, strerror(errno));
			return -1;
		}

		events[i] = epollStruct;
	}
	// }
	// struct epoll_event epollStruct;
	// epollStruct.events = EPOLLIN | EPOLLET | EPOLLPRI;
	// epollStruct.data.fd = fdList[0];
	// ignore first trigger
	for (int i = 0; i < 2; i++) {
		int waitRet = epoll_wait(
				epollfd, 
				events, 
				NUM_DIRECTIONS,                // maximum # events
				5000);              // timeout in ms, -1 = wait indefinite; 0 = returne immediately

		if (waitRet == -1){
			fprintf(stderr, "ERROR: epoll_wait() failed (%d) = %s\n", waitRet, strerror(errno));
			for (int i = 0; i < NUM_DIRECTIONS; i++){
				close(fdList[i]);
			}
			// close(fdLeft);
			close(epollfd);
			return -1;
		} else if (waitRet == 0) {
			return 1;
		}
	}

    printf("debug 1\n");
	// cleanup epoll instance:
	for (int i = 0; i < NUM_DIRECTIONS; i++){
		close(fdList[i]);
	}
	close(epollfd);
	return 0;
}

// int main() 
// {
// 	// Assume joystick pin already exported as GPIO
// 	// .. set direction of GPIO pin to input
//     for (int i = 0; i < NUM_DIRECTIONS; i++){
//         writeToFile(DirectionFiles[i], "in");
//         // .. set edge trigger; options are "none", "rising", "falling", "both"
//         writeToFile(EdgeFiles[i], "both");
//     }

// 	// Block and wait for edge triggered change on GPIO pin
// 	// printf("Now waiting on input for file: %s\n", JSLFT_IN);
// 	while (1) {

// 		// Wait for an edge trigger:
// 		int ret = waitForGpioEdge(ValueFiles);
// 		if (ret == -1) {
// 			exit(EXIT_FAILURE);
// 		} else {
// 			printf("number of events triggered: %d\n", ret);
// 		}

// 		for (int i = 0; i < NUM_DIRECTIONS; i++){
// 			// Current state:
// 			char buff[1024];
// 			// char* file = ValueFiles[i];
// 			int bytesRead = readLineFromFile(ValueFiles[i], buff, 1024);
// 			if (bytesRead > 0) {
// 				printf("GPIO pin %d reads: %c\n", i, buff[0]);
// 			} else {
// 				fprintf(stderr, "ERROR: Read 0 bytes from GPIO input: %s\n", strerror(errno));
// 			}
// 		}
// 	}
// 	return 0;
// }