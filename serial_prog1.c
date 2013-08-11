#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdlib.h>

#define STX 0x02
#define ETX 0x03
#define ACK 0x6

#define READ_DISPLAY 0x11
#define WRITE_DISPLAY 0x21
#define CONTRAST_REG 0x09
#define BRIGHTNESS_REG 0x08
#define DATA_LEN 0x01
#define TEST_PATTERN_REG 0x4a
#define PATTERN 0x03

#define MIN_VALUE 0x00
#define MAX_VALUE 0xff

#define DEVICE_NAME "/dev/ttyUSB0"

static int fd=0;
int open_port()
{
	struct termios options;
	fd = open(DEVICE_NAME,O_RDWR | O_NDELAY | O_NOCTTY | O_NONBLOCK);
	if (fd == -1)
	{
		perror("Can't open device\n");
	}
	else
	{
		fcntl(fd, F_SETFL,0);
		printf("Open Successfull\n");
		/* Setting port parameters */
		tcgetattr(fd, &options);
		/* control flags */
		cfsetispeed(&options,B19200);    /* 9600 Bds   */
		options.c_cflag &= ~PARENB;     /* No parity  */
		options.c_cflag &= ~CSTOPB;     /*            */
		options.c_cflag &= ~CSIZE;      /* 8bit       */
		options.c_cflag |= CS8;         /*            */
		options.c_cflag &= ~CRTSCTS;    /* No hdw ctl */

		/* local flags */
		options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* raw input */

		/* input flags */
		/*
		   iface->options.c_iflag &= ~(INPCK | ISTRIP); // no parity
		   iface->options.c_iflag &= ~(IXON | IXOFF | IXANY); // no soft ctl
		 */
		/* patch: bpflegin: set to 0 in order to avoid invalid pan/tilt return val
		   ues */
		options.c_iflag = 0;
		/* output flags */
		options.c_oflag &= ~OPOST; /* raw output */

		tcsetattr(fd, TCSANOW, &options);
		tcflush(fd, TCIFLUSH);
		tcsetattr(fd, TCSANOW, &options);

	}
	return (fd);
}
int read_port(char *buf,int length)
{
	int len = read(fd, buf, length);
	if(len < 0)
		printf("No data\n");
	return len;

}
int write_port(char *buff,int len)
{
	if ( fd < 0)
		return -1;
	// autocalculate if using short form
	if (len == 0)
		len = strlen(buff);
	// ugly error handling, if write fails then shut down unit
	if (write(fd, buff, len) < len) {
		fprintf(stderr,"Error writing data\n");
		//        Disconnect();
		return -1;
	}

	return 0;
}

void delay(void)
{
	int i,j;
	for(i=0; i<65530; i++)
	{
		for(j=0; j<10; j++)
		{
			;
		}
	}
}
int main(){
	int fd1,ret;
	char buf[2];
	while(1)
	{
		buf[0]='h'; buf[1]='i';
		fd1=open_port();
		if(fd > 0)
		{
			write_port(buf,255);
			delay();
			close(fd);
		}
		else
		{
			printf("Error in Open_port()\n");
		}
	}
	return 0;
}

