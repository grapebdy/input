#ifndef __COMMON_H__
#define __COMMON_H__

#define VT_MOUSE_BT_LEFTT	_IOWR('H',1,int)
#define VT_MOUSE_BT_RIGHT	_IOWR('H',2,int)
#define VT_MOUSE_BT_MIDDL	_IOWR('H',3,int)

#define VT_MOUSE_REL		_IOWR('R',1,int)

#define VT_KEYBOARD_KEY		_IOWR('K',1,int)

typedef struct virtkey_msg {
	int relx;
	int rely;

	int keycode;
	int pressup;
}virt_msg;

#endif
