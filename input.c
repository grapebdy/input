#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/input.h>
#include <linux/bitops.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/list.h>

#define  BUTTON_IRQ 0xb

struct scan_code {
	unsigned char keycode;
	struct list_head list;
};

struct virt_device {
	struct input_dev *button_dev;
	struct work_struct input_work;
	struct workqueue_struct *input_workqueue;
	struct bin_attribute bin;
	struct list_head code_list;
};

struct virt_device virt_dev;

static unsigned char button_keycode[0x72] = {	/* American layout */
	[0]	 = KEY_GRAVE,
	[1]	 = KEY_ESC,
	[2]	 = KEY_1,
	[3]	 = KEY_2,
	[4]	 = KEY_3,
	[5]	 = KEY_4,
	[6]	 = KEY_5,
	[7]	 = KEY_6,
	[8]	 = KEY_7,
	[9]	 = KEY_8,
	[10]	 = KEY_9,
	[11]	 = KEY_0,
	[12]	 = KEY_MINUS,
	[13]	 = KEY_EQUAL,
	[14]	 = KEY_BACKSPACE,
	[15]	 = KEY_TAB,
	[16]	 = KEY_Q,
	[17]	 = KEY_W,
	[18]	 = KEY_E,
	[19]	 = KEY_R,
	[20]	 = KEY_T,
	[21]	 = KEY_Y,
	[22]	 = KEY_U,
	[23]	 = KEY_I,
	[24]	 = KEY_O,
	[25]	 = KEY_P,
	[26]	 = KEY_LEFTBRACE,
	[27]	 = KEY_RIGHTBRACE,
	[28]	 = KEY_ENTER,
	[29]	 = KEY_LEFTCTRL,
	[30]	 = KEY_A,
	[31]	 = KEY_S,
	[32]	 = KEY_D,
	[33]	 = KEY_F,
	[34]	 = KEY_G,
	[35]	 = KEY_H,
	[36]	 = KEY_J,
	[37]	 = KEY_K,
	[38]	 = KEY_L,
	[39]	 = KEY_SEMICOLON,
	[40]	 = KEY_APOSTROPHE,
	[41]	 = KEY_BACKSLASH,	/* FIXME, '#' */
	[42]	 = KEY_LEFTSHIFT,
	[43]	 = KEY_GRAVE,		/* FIXME: '~' */
	[44]	 = KEY_Z,
	[45]	 = KEY_X,
	[46]	 = KEY_C,
	[47]	 = KEY_V,
	[48]	 = KEY_B,
	[49]	 = KEY_N,
	[50]	 = KEY_M,
	[51]	 = KEY_COMMA,
	[52]	 = KEY_DOT,
	[53]	 = KEY_SLASH,
	[54]	 = KEY_RIGHTSHIFT,
	[55]	 = KEY_KPASTERISK,
	[56]	 = KEY_LEFTALT,
	[57]	 = KEY_SPACE,
	[58]	 = KEY_CAPSLOCK,
	[59]	 = KEY_F1,
	[60]	 = KEY_F2,
	[61]	 = KEY_F3,
	[62]	 = KEY_F4,
	[63]	 = KEY_F5,
	[64]	 = KEY_F6,
	[65]	 = KEY_F7,
	[66]	 = KEY_F8,
	[67]	 = KEY_F9,
	[68]	 = KEY_F10,
	[69]	 = KEY_ESC,
	[70]	 = KEY_DELETE,
	[71]	 = KEY_KP7,
	[72]	 = KEY_KP8,
	[73]	 = KEY_KP9,
	[74]	 = KEY_KPMINUS,
	[75]	 = KEY_KP4,
	[76]	 = KEY_KP5,
	[77]	 = KEY_KP6,
	[78]	 = KEY_KPPLUS,
	[79]	 = KEY_KP1,
	[80]	 = KEY_KP2,
	[81]	 = KEY_KP3,
	[82]	 = KEY_KP0,
	[83]	 = KEY_KPDOT,
	[90]	 = KEY_KPLEFTPAREN,
	[91]	 = KEY_KPRIGHTPAREN,
	[92]	 = KEY_KPASTERISK,	/* FIXME */
	[93]	 = KEY_KPASTERISK,
	[94]	 = KEY_KPPLUS,
	[95]	 = KEY_HELP,
	[96]	 = KEY_BACKSLASH,	/* FIXME: '<' */
	[97]	 = KEY_KPASTERISK,	/* FIXME */
	[98]	 = KEY_KPSLASH,
	[99]	 = KEY_KPLEFTPAREN,
	[100]	 = KEY_KPRIGHTPAREN,
	[101]	 = KEY_KPSLASH,
	[102]	 = KEY_KPASTERISK,
	[103]	 = KEY_UP,
	[104]	 = KEY_KPASTERISK,	/* FIXME */
	[105]	 = KEY_LEFT,
	[106]	 = KEY_RIGHT,
	[107]	 = KEY_KPASTERISK,	/* FIXME */
	[108]	 = KEY_DOWN,
	[109]	 = KEY_KPASTERISK,	/* FIXME */
	[110]	 = KEY_KPASTERISK,	/* FIXME */
	[111]	 = KEY_KPASTERISK,	/* FIXME */
	[112]	 = KEY_KPASTERISK,	/* FIXME */
	[113]	 = KEY_KPASTERISK	/* FIXME */
};

unsigned int intr_lock = 0;
module_param(intr_lock, uint, 0644);

static irqreturn_t button_interrupt(int irq,void *dev_instance)
{
	if (intr_lock) {
		intr_lock = 0;
		input_report_key(virt_dev.button_dev, KEY_F5, 1);
		input_report_key(virt_dev.button_dev, KEY_F5, 0);
		input_sync(virt_dev.button_dev);
	}
	return IRQ_RETVAL(1);
}

static void input_work_routine(struct work_struct *work)
{
	struct scan_code *code_tmp;

	printk("input test \n");

	while (!list_empty(virt_dev.code_list.next)) {
		code_tmp = list_entry(virt_dev.code_list.next, struct scan_code, list);
		printk("queue: %x \n", code_tmp->keycode);

		input_report_key(virt_dev.button_dev, button_keycode[code_tmp->keycode], 1);
		input_report_key(virt_dev.button_dev, button_keycode[code_tmp->keycode], 0);
		input_sync(virt_dev.button_dev);
		list_del(virt_dev.code_list.next);
		kfree(code_tmp);
	}

}

static ssize_t input_bin_write(struct file *filp, struct kobject *kobj,
                struct bin_attribute *attr,
                char *buf, loff_t off, size_t count)
{
//	struct scan_code *code_tmp;
	struct scan_code *code = (struct scan_code *)kmalloc(sizeof(struct scan_code), GFP_KERNEL);
	if (!code) {
		printk("write error no buff \n");
		return 1;
	}
	printk("buff: %x\n", buf[0]);
	code->keycode = buf[0];
	list_add_tail(&code->list, &(virt_dev.code_list));
/*
	list_for_each_entry(code_tmp, &(virt_dev.code_list), list)
		printk("LQQ-> %d \n", code_tmp->keycode);
*/
	queue_work(virt_dev.input_workqueue, &(virt_dev.input_work));
	return 1;
}

static ssize_t input_bin_read(struct file *filp, struct kobject *kobj,
                struct bin_attribute *attr,
                char *buf, loff_t off, size_t count)
{
	return 0;
}

static int __init button_init(void)
{
	int error;
	int i;

	virt_dev.button_dev = input_allocate_device();
	if (!virt_dev.button_dev) {
		printk(KERN_ERR"input.c: Not enough memory\n");
		return  -ENOMEM;
	}

	if (request_irq(BUTTON_IRQ, button_interrupt, IRQF_SHARED, "button", virt_dev.button_dev)) {
		printk(KERN_ERR"input.c: Can't allocate irq %d\n", BUTTON_IRQ);
		goto err_free_dev;
	}

	virt_dev.button_dev->name = "button";
	virt_dev.button_dev->phys = "lqq/button";
        virt_dev.button_dev->id.bustype = BUS_HOST;
        virt_dev.button_dev->id.vendor = 0x0001;
        virt_dev.button_dev->id.product = 0x0001;
        virt_dev.button_dev->id.version = 0x0100;

	set_bit(EV_REP, virt_dev.button_dev->evbit);
	set_bit(EV_KEY, virt_dev.button_dev->evbit);
	for (i = 0; i < 0x72; i++)
		set_bit(button_keycode[i], virt_dev.button_dev->keybit);

	error = input_register_device(virt_dev.button_dev);
	if (error) {
		printk(KERN_ERR"input.c: Failed to register device\n");
		goto err_free_irq;
	}

	virt_dev.input_workqueue = alloc_workqueue("button", WQ_MEM_RECLAIM | WQ_HIGHPRI, 0);
	if (!virt_dev.input_workqueue) {
		printk("workqueue failed, use interrupt mode\n");
		goto err_unreg_dev;
	}

	INIT_WORK(&(virt_dev.input_work),input_work_routine);
//	queue_work(virt_dev.input_workqueue, &(virt_dev.input_work));
	sysfs_bin_attr_init(&virt_dev.bin);
	virt_dev.bin.attr.name = "virtkey";
	virt_dev.bin.attr.mode = S_IRUGO | S_IWUSR;
	virt_dev.bin.read = input_bin_read;
	virt_dev.bin.write = input_bin_write;
	virt_dev.bin.size = 256;
	error = sysfs_create_bin_file(&virt_dev.button_dev->dev.kobj, &virt_dev.bin);
	if (error)
		goto err_unreg_queue;

	INIT_LIST_HEAD(&(virt_dev.code_list));

	return 0;

err_unreg_queue:
	destroy_workqueue(virt_dev.input_workqueue);
err_unreg_dev:
	input_unregister_device(virt_dev.button_dev);
err_free_irq:
	free_irq(BUTTON_IRQ, virt_dev.button_dev);
err_free_dev:
	input_free_device(virt_dev.button_dev);
	return error;
}

static void __exit button_exit(void)
{
	input_unregister_device(virt_dev.button_dev);
	input_free_device(virt_dev.button_dev);
	free_irq(BUTTON_IRQ, virt_dev.button_dev);
	if (!virt_dev.input_workqueue)
		destroy_workqueue(virt_dev.input_workqueue);
}

module_init(button_init);
module_exit(button_exit);

MODULE_AUTHOR("LiangQiangqiang");
MODULE_DESCRIPTION("PKUNITY-3 SOC input driver");
MODULE_LICENSE("GPL v2");
