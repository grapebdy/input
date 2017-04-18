#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>

#include "../include/common.h"

#define MOUSE_FILE "/dev/virtmouse"

int keyboard_alt_click(int keycode)
{
        int fd = -1;
        int rd_num;
	virt_msg msg1;

        fd = open(MOUSE_FILE, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", MOUSE_FILE);
                return fd;
        }
	msg1.keycode  = KEY_LEFTALT;
	msg1.pressup = 1;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

	msg1.keycode  = keycode;
	msg1.pressup = 1;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

	msg1.keycode  = keycode;
	msg1.pressup = 0;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

	msg1.keycode  = KEY_LEFTALT;
	msg1.pressup = 0;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

        close(fd);
        return rd_num;

}
int keyboard_ctl_click(int keycode)
{

        int fd = -1;
        int rd_num;
	virt_msg msg1;

        fd = open(MOUSE_FILE, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", MOUSE_FILE);
                return fd;
        }
	msg1.keycode  = KEY_LEFTCTRL;
	msg1.pressup = 1;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

	msg1.keycode  = keycode;
	msg1.pressup = 1;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

	msg1.keycode  = keycode;
	msg1.pressup = 0;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

	msg1.keycode  = KEY_LEFTCTRL;
	msg1.pressup = 0;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

        close(fd);
        return rd_num;
}

int keyboard_normal_click(int keycode)
{

        int fd = -1;
        int rd_num;
	virt_msg msg1;

        fd = open(MOUSE_FILE, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", MOUSE_FILE);
                return fd;
        }
	msg1.keycode  = keycode;
	msg1.pressup = 1;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

	msg1.keycode  = keycode;
	msg1.pressup = 0;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

        close(fd);
        return rd_num;
}



int keyboard_shift_click(int keycode)
{

        int fd = -1;
        int rd_num;
	virt_msg msg1;

        fd = open(MOUSE_FILE, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", MOUSE_FILE);
                return fd;
        }
	msg1.keycode  = KEY_LEFTSHIFT;
	msg1.pressup = 1;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

	msg1.keycode  = keycode;
	msg1.pressup = 1;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

	msg1.keycode  = keycode;
	msg1.pressup = 0;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

	msg1.keycode  = KEY_LEFTSHIFT;
	msg1.pressup = 0;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg1);

        close(fd);
        return rd_num;
}

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
	virt_msg msg;
        fd = open(filename, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", filename);
                return fd;
        }
        msg.relx = 1;
        msg.rely = 2;
        rd_num = read(fd, &buff, 1);
        rd_num = write(fd, &buff, 1);
	//rd_num = ioctl(fd, VT_MOUSE_BT_LEFTT, &buff);
	//rd_num = ioctl(fd, VT_MOUSE_BT_RIGHT, &buff);
	//rd_num = ioctl(fd, VT_MOUSE_BT_MIDDL, &buff);
	//rd_num = ioctl(fd, VT_MOUSE_REL, &buff);

        msg.keycode = 3;
        msg.pressup = 1;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg);

        msg.keycode = 3;
        msg.pressup = 0;
	rd_num = ioctl(fd, VT_KEYBOARD_KEY, &msg);

        close(fd);
        return rd_num;
}

void do_usage(void)
{
        printf("Usage: virtkey cmd [OPERAND]...\n");
        printf("EEPROM test program\n");
        printf("Command:\n");
        printf("\t-k keycode\t\t Write key code normal\n");
        printf("\t-s keycode\t\t Write key code with shit\n");
        printf("\t-c keycode\t\t Write key code with ctl\n");
        printf("\t-a keycode\t\t Write key code with alt\n");
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

	if (argc < 2) {
		do_usage();
		return -1;
	}
	while ((opt = getopt(argc,argv,"a:c:s:k:trlmnxy")) !=-1) {
		switch (opt) {
		case 'a':
			keycode = atoi(optarg);
			keyboard_alt_click(keycode);
			return 0;
		case 'c':
			keycode = atoi(optarg);
			keyboard_ctl_click(keycode);
			return 0;
		case 'k':
			keycode = atoi(optarg);
			keyboard_normal_click(keycode);
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
		case 's':
			keycode = atoi(optarg);
			keyboard_shift_click(keycode);
			return 0;
		default:
			do_usage();
			return -1;
		}
	}

	return 0;
}
