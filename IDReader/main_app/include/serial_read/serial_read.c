#include "serial_read.h"
#include "stdio.h"

int serial_open(const char * device, uint32_t baud_rate)
{
    int fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd == -1)
    {
        perror(device);
        return -1;
    }

    int result = tcflush(fd, TCIOFLUSH);
    if(result)
    {
        perror("Failed to flush\n");
        return -1;
    }

    struct termios options;
    result = tcgetattr(fd, &options);
    if(result)
    {
        perror("Failed to get attribute\n");
        close(fd);
        return -1;
    }

    options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
    options.c_oflag &= ~(ONLCR | OCRNL);
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

    options.c_cc[VTIME] = 1;
    options.c_cc[VMIN] = 0;

    cfsetospeed(&options, B115200);
    cfsetispeed(&options, cfgetospeed(&options));

    result = tcsetattr(fd, TCSANOW, &options);
    if (result)
    {
        perror("tcsetattr failed");
        close(fd);
        return -1;
    }

    return fd;
}

int write_port(int fd, uint8_t * buffer, size_t size)
{
  ssize_t result = write(fd, buffer, size);
  if (result != (ssize_t)size)
  {
    perror("failed to write to port");
    return -1;
  }
  return 0;
}

ssize_t read_port(int fd, uint8_t * buffer, size_t size)
{
  size_t received = 0;
  while (received < size)
  {
    ssize_t r = read(fd, buffer + received, size - received);
    if (r < 0)
    {
      perror("failed to read from port");
      return -1;
    }
    if (r == 0)
    {
      // Timeout
      break;
    }
    received += r;
  }
  return received;
}

void read_example(void)
{
    const char * device = DEVICE_PATH;

    printf("Opening port\n");

    int fd = serial_open(device, BAUD_RATE);
    if(fd < 0)
    {
        printf("Failed to open device!\n");
    }
    printf("Connection successful!\n");
    int messages_received = 100;
    int ctr = 0;
    while(1)
    {
        uint8_t data[BUF_SIZE];
        size_t msg_len = read_port(fd, data, BUF_SIZE);
        if(msg_len > 0)
        {
            for(int i = 0; i < msg_len; i++)
            {
                printf("%c", data[i]);
            }
            ctr++;
        }
        if(ctr >= messages_received)
        {
            break;
        }
    }

    close(fd);
}