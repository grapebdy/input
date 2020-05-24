/*
 * virtkeylib.c - A series of input api.
 *
 * Copyright (c) 2020-2020. All Rights Reserved.
 *
 * Author: qqliang<grapel2013@gmail.com>
 *
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <virtkey.h>
#include <linux/input.h>

/*
 * keyboard_alt_click:
 *
 * Returns 0 on success, -1 on failure
 */
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

/*
 * keyboard_ctl_click:
 *
 * Returns 0 on success, -1 on failure
 */
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

/*
 * keyboard_normal_click:
 *
 * Returns 0 on success, -1 on failure
 */
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

/*
 * keyboard_shift_click:
 *
 * Returns 0 on success, -1 on failure
 */
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

/*
 * key_write:
 *
 * Returns 0 on success, -1 on failure
 */
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

/*
 * mouse_middle_rel:
 *
 * Returns 0 on success, -1 on failure
 */
int mouse_middle_rel(int relx, int rely)
{

        int fd = -1;
        int rd_num;
	virt_msg msg;
        fd = open(MOUSE_FILE, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", MOUSE_FILE);
                return fd;
        }
	msg.relx = relx;
	msg.rely = rely;
	rd_num = ioctl(fd, VT_MOUSE_REL, &msg);
        close(fd);
        return rd_num;
}

/*
 * mouse_middle_click:
 *
 * Returns 0 on success, -1 on failure
 */
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

/*
 * mouse_right_click:
 *
 * Returns 0 on success, -1 on failure
 */
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

/*
 * mouse_left_click:
 *
 * Returns 0 on success, -1 on failure
 */
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

/*
 * mouse_options:
 *
 * Returns 0 on success, -1 on failure
 */
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
