/*
    Used for reading and writing to selected serial port
*/
#ifndef SERIAL_READ_H
#define SERIAL_READ_H

#include "stdint.h"
#include "stddef.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define DEVICE_PATH "/dev/tty.usbmodemF412FA6F2EA42"

#ifndef BAUD_RATE
    #define BAUD_RATE 115200
#endif

#ifndef BUF_SIZE
    #define BUF_SIZE 4096
#endif /* BUF_SIZE */

int serial_open(const char * device, uint32_t baud_rate);
int write_port(int fd, uint8_t * buffer, size_t size);
ssize_t read_port(int fd, uint8_t * buffer, size_t size);
void read_example(void);







#endif