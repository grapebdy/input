#ifndef __COMMON_H__
#define __COMMON_H__

#define VT_MOUSE_BT_LEFTT	_IOWR('H',1,int)
#define VT_MOUSE_BT_RIGHT	_IOWR('H',2,int)
#define VT_MOUSE_BT_MIDDL	_IOWR('H',3,int)
#define VT_MOUSE_REL		_IOWR('R',1,int)
#define VT_KEYBOARD_KEY		_IOWR('K',1,int)

#define MOUSE_FILE "/dev/virtmouse"

typedef struct virtkey_msg {
	int relx;
	int rely;

	int keycode;
	int pressup;
}virt_msg;

int keyboard_alt_click(int keycode);
int keyboard_ctl_click(int keycode);
int keyboard_normal_click(int keycode);
int keyboard_shift_click(int keycode);
int key_write(const char *filename, unsigned char buff);
int mouse_middle_rel(int relx, int rely);
int mouse_middle_click(const char *filename);
int mouse_right_click(const char *filename);
int mouse_left_click(const char *filename);
int mouse_options(const char *filename, unsigned char buff);
#endif
