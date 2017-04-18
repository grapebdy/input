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

struct keymap_detail {
	char type[10];
	int keycode;
}keymap[200] = {
{"`",0}, {"ESC",1}, {"1",2}, {"2",3}, {"3",4}, {"4",5}, {"5",6}, {"6",7}, {"7",8}, {"8",9},
{"9",10}, {"0",11}, {"-",12}, {"=",13}, {"<-",14}, {"TAB",15}, {"q",16}, {"w",17}, {"e",18}, {"r",19},
{"t",20}, {"y",21}, {"u",22}, {"i",23}, {"o",24}, {"p",25}, {"[",26}, {"]",27}, {"enter",28}, {"leftctl",29},
{"a",30}, {"s",31}, {"d",32}, {"f",33}, {"g",34}, {"h",35}, {"j",36}, {"k",37}, {"l",38}, {";",39},
{"'",40}, {"\\",41}, {"leftshit",42}, {"",43}, {"z",44}, {"x",45}, {"c",46}, {"v",47}, {"b",48}, {"n",49},
{"m",50}, {",",51}, {".",52}, {"/",53}, {"rightshift",54}, {"*",55}, {"leftatl",56}, {"space",57}, {"caplock",58}, {"F1",59},

{"F2",60}, {"F3",61}, {"F4",62}, {"F5",63}, {"F6",64}, {"F7",65}, {"F8",66}, {"F9",67}, {"F10",68}, {"ESC",69},
{"del",70}, {"kp7",71}, {"kp8",72}, {"kp9",73}, {"-",74}, {"kp4",75}, {"kp5",76}, {"kp6",77}, {"+",78}, {"kp1",79},
{"kp2",80}, {"kp3",81}, {"kp0",82}, {".",83}, {"",84}, {"",85}, {"",86}, {"",87}, {"",88}, {"",89},

{"",80}, {"kp3",81}, {"kp0",82}, {".",83}, {"",84}, {"",85}, {"",86}, {"",87}, {"",88}, {"",89},
{"",90}, {"",91}, {"*",92}, {"*",83}, {"+",94}, {"help",95}, {"\\",96}, {"*",97}, {"/",98}, {"",99},
{"",100}, {"/",101}, {"*",102}, {"up",103}, {"*",104}, {"left",105}, {"right",106}, {"*",107}, {"down",108}, {"*",109},
{"*",110}, {"*",111}, {"*",112}, {"*",113}
};
void keymap_detail(void)
{
	int i;
	for (i = 0; i < 114; i++)
		printf("%10s:%d\t%c", keymap[i].type, keymap[i].keycode, i % 4 == 3?'\n':' ');
	printf("\n");
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
        printf("\t-r        \t\t mouse right click\n");
        printf("\t-l        \t\t mouse left  click\n");
        printf("\t-m        \t\t mouse middle click\n");
        printf("\t-n        \t\t mouse mov  pixel\n");
        printf("\t-x xposion\t\t mouse move x position\n");
        printf("\t-y yposion\t\t mouse move y position\n");
        printf("\t-v        \t\t show the keyboard map\n");

}

int main(int argc, char *argv[])
{
	int opt;
	int keycode =28; // enter

	if (argc < 2) {
		do_usage();
		return -1;
	}
	while ((opt = getopt(argc,argv,"a:c:s:k:trlmnxyv")) !=-1) {
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
		case 'v':
			keymap_detail();
			return 0;
		default:
			do_usage();
			return -1;
		}
	}

	return 0;
}
