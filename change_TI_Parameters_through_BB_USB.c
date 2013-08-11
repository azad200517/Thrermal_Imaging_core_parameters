#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>

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
	int fd1,val;
	char msg[10];
	while(1)
	{
		// Te set brightness
		msg[0] = 0x53;
		msg[1] = 0x00;
		msg[2] = 0x8A; 
		msg[3] = 0x00; 
		msg[4] = 0x00; 
		msg[5] = (int)(val/256); 
		msg[6] = val%256;
		val = msg[1] +msg[2] +msg[3] +msg[4] +msg[5] +msg[6];
		val = val % 0xFF;
		msg[7] = val;
		msg[8] = 0x45;
		msg[9] = '\r';

		fd1=open_port();
		if(fd1 > 0)
                {
                        write_port(msg,10);
                //        delay();
                        close(fd1);
                }
                else
                {
                        printf("Error in Open_port()\n");
                }
        }
        return 0;
}


