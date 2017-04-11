#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../include/common.h"

#define MOUSE_FILE "/dev/virtmouse"

int key_write(const char *filename, unsigned char buff)
{
        int fd = -1;
        int rd_num;
        fd = open(filename, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", filename);
                return fd;
        }

        rd_num = write(fd, &buff, 1);

        close(fd);
        return rd_num;
}


int mouse_middle_rel(const char *filename)
{

        int fd = -1;
        int rd_num;
	char buff[4];
        fd = open(filename, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", filename);
                return fd;
        }
	rd_num = ioctl(fd, VT_MOUSE_REL, buff);
        close(fd);
        return rd_num;
}

int mouse_middle_click(const char *filename)
{

        int fd = -1;
        int rd_num;
	char buff[4];
        fd = open(filename, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", filename);
                return fd;
        }
	rd_num = ioctl(fd, VT_MOUSE_BT_MIDDL, buff);
        close(fd);
        return rd_num;
}

int mouse_right_click(const char *filename)
{

        int fd = -1;
        int rd_num;
	char buff[4];
        fd = open(filename, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", filename);
                return fd;
        }
	rd_num = ioctl(fd, VT_MOUSE_BT_RIGHT, buff);
        close(fd);
        return rd_num;
}

int mouse_left_click(const char *filename)
{

        int fd = -1;
        int rd_num;
	char buff[4];
        fd = open(filename, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", filename);
                return fd;
        }

	rd_num = ioctl(fd, VT_MOUSE_BT_LEFTT, buff);

        close(fd);
        return rd_num;


}

int mouse_options(const char *filename, unsigned char buff)
{
        int fd = -1;
        int rd_num;
        fd = open(filename, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", filename);
                return fd;
        }

        rd_num = write(fd, &buff, 1);
        rd_num = read(fd, &buff, 1);
      //  rd_num = ioctl(fd, VT_MOUSE_BT_LEFTT, &buff);
      //  rd_num = ioctl(fd, VT_MOUSE_BT_RIGHT, &buff);
      //  rd_num = ioctl(fd, VT_MOUSE_BT_MIDDL, &buff);
	rd_num = ioctl(fd, VT_MOUSE_REL, &buff);

        close(fd);
        return rd_num;
}

void do_usage(void)
{
        printf("Usage: virtkey cmd [OPERAND]...\n");
        printf("EEPROM test program\n");
        printf("Command:\n");
        printf("\t-f filename\t\t virtkey input file(dmesg)\n");
        printf("\t-k keycode\t\t Write key code\n");
        printf("\t-r \t\t mouse right click\n");
        printf("\t-l \t\t mouse left  click\n");
        printf("\t-m \t\t mouse middle click\n");
        printf("\t-n\t\t mouse mov  pixel\n");
        printf("\t-x xposion\t\t mouse move x position\n");
        printf("\t-y yposion\t\t mouse move y position\n");

}

int main(int argc, char *argv[])
{
	int opt;
	int keycode =28; // enter
	int fileno = 4;	 // input device 4
	char filename[128];
	if (argc < 3) {
		do_usage();
		return -1;
	}

	while ((opt = getopt(argc,argv,"f:k:trlmnxy")) !=-1) {
		switch (opt) {
		case 'f':
			fileno = atoi(optarg);
			break;
		case 'k':
			keycode = atoi(optarg);
			break;
		case 't':
			mouse_options(MOUSE_FILE, 10);
			return 0;
		case 'r':
			mouse_right_click(MOUSE_FILE);
			break;
		case 'l':
			mouse_left_click(MOUSE_FILE);
			break;
		case 'm':
			mouse_middle_click(MOUSE_FILE);
			break;
		case 'x':
			mouse_middle_rel(MOUSE_FILE);
			break;
		default:
			do_usage();
			return -1;
		}
	}

	sprintf(filename, "%s%d%s", "/sys/devices/virtual/input/input", fileno, "/virtkey");
	printf("%s:%d \n", filename, keycode);
	key_write(filename, keycode);

	return 0;
}
