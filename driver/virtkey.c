#include "virtkey.h"

struct virt_device virt_dev;

unsigned int intr_lock = 0;
module_param(intr_lock, uint, 0644);
unsigned int debug_level = 0;
module_param(debug_level, uint, 0644);

static irqreturn_t button_interrupt(int irq,void *dev_instance)
{
	if (intr_lock) {
		intr_lock = 0;
		printk("interrupt\n");
//		input_report_key(virt_dev.button_dev, BTN_LEFT, 1);
//		input_report_key(virt_dev.button_dev, BTN_LEFT, 0);
		input_report_rel(virt_dev.button_dev, REL_X, 10);
		input_report_rel(virt_dev.button_dev, REL_Y, 10);
		input_sync(virt_dev.button_dev);
	}
	return IRQ_RETVAL(1);
}

static void input_work_routine(struct work_struct *work)
{
	struct scan_code *code_tmp;

	while (!list_empty(virt_dev.code_list.next)) {

		code_tmp = list_entry(virt_dev.code_list.next, struct scan_code, list);
		printk("workqueue input data: %x \n", code_tmp->keycode);

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
	struct scan_code *code;

	code  = (struct scan_code *)kmalloc(sizeof(struct scan_code), GFP_KERNEL);
	if (!code) {
		printk("write error no buff \n");
		return 1;
	}

	if (buf[0] > KEY_MAP_SIZE)
		code->keycode = 0;
	else
		code->keycode = buf[0];
	list_add_tail(&code->list, &(virt_dev.code_list));

	queue_work(virt_dev.input_workqueue, &(virt_dev.input_work));
	return 1;
}

static ssize_t input_bin_read(struct file *filp, struct kobject *kobj,
                struct bin_attribute *attr,
                char *buf, loff_t off, size_t count)
{
	return 0;
}

#define DEV_MAJOR_NUM 250
#define DEV_MINOR_NUM 1

int virt_mouse_open(struct inode *fnode, struct file *fp)
{
        return 0;
}

int virt_mouse_release(struct inode *fnod, struct file *filp)
{
        return 0;
}
ssize_t virt_mouse_read(struct file *filp, char __user *buffer, size_t size, loff_t *ppos)
{
	PDEBUG("%s \n", __func__);
	return 0;
}

ssize_t virt_mouse_write(struct file *filp, const char __user *buffer, size_t size, loff_t *ppos)
{
	PDEBUG("%s \n", __func__);
	return 0;
}

long virt_mouse_ioctl(struct file *filp, unsigned int cmd , unsigned long arg)
{
	virt_msg *msg = (virt_msg *)arg;
	switch(cmd) {
	case VT_MOUSE_BT_LEFTT:
		PDEBUG("%s: VT_MOUSE_BT_LEFTT \n", __func__);
		input_report_key(virt_dev.button_dev, BTN_LEFT, 1);
		input_report_key(virt_dev.button_dev, BTN_LEFT, 0);
		input_sync(virt_dev.button_dev);
		break;
	case VT_MOUSE_BT_RIGHT:
		PDEBUG("%s: VT_MOUSE_BT_RIGHT \n", __func__);
		input_report_key(virt_dev.button_dev, BTN_RIGHT, 1);
		input_report_key(virt_dev.button_dev, BTN_RIGHT, 0);
		input_sync(virt_dev.button_dev);
		break;
	case VT_MOUSE_BT_MIDDL:
		PDEBUG("%s: VT_MOUSE_BT_MIDDL \n", __func__);
		input_report_key(virt_dev.button_dev, BTN_MIDDLE, 1);
		input_report_key(virt_dev.button_dev, BTN_MIDDLE, 0);
		input_sync(virt_dev.button_dev);
		break;
	case VT_MOUSE_REL:
		PDEBUG("%s: VT_MOUSE_BT_REL relx:%d, rely: %d\n",
					__func__, msg->relx, msg->rely);
		input_report_rel(virt_dev.button_dev, REL_X, msg->relx);
		input_report_rel(virt_dev.button_dev, REL_Y, msg->rely);
		input_sync(virt_dev.button_dev);
		break;
	case VT_KEYBOARD_KEY:
		PDEBUG("%s: VT_KEYBOARD_KEY %d: %d\n",
			__func__, msg->keycode, msg->pressup);

		input_report_key(virt_dev.button_dev,
			button_keycode[msg->keycode], msg->pressup);
		input_sync(virt_dev.button_dev);
		break;
	default:
		printk("%s: COMMAND is not available\n", __func__);
	}

	return 0;
}

static const struct  file_operations virtmouse_ops = {
        .open   = virt_mouse_open,
        .read   = virt_mouse_read,
        .write  = virt_mouse_write,
        .unlocked_ioctl = virt_mouse_ioctl,
};
static int register_virtmouse(void)
{
	int ret;
	struct device *dev = NULL;

	virt_dev.virtmouse.devno = MKDEV(DEV_MAJOR_NUM, DEV_MINOR_NUM);
	ret = register_chrdev_region(virt_dev.virtmouse.devno, 1, "virtmouse");
	if(ret < 0) {
		printk("%s: request_chrdev_region failed,ret=0x%x\n",__func__,ret);
		return -1;
	}
	cdev_init(&(virt_dev.virtmouse.mousecdev), &virtmouse_ops);
	virt_dev.virtmouse.mousecdev.owner = THIS_MODULE;
	ret = cdev_add(&(virt_dev.virtmouse.mousecdev), virt_dev.virtmouse.devno, 1);
	if (ret) {
		printk("%s: cdev add failed ret=0x%x \n",__func__,ret);
		goto unreg_region;
	}
	virt_dev.virtmouse.virt_mouse_class = class_create(THIS_MODULE,"virt_mouse_class");
        if(IS_ERR(virt_dev.virtmouse.virt_mouse_class)) {
                printk("create class failed \n");
		goto unreg_cdev;
        }

	dev = device_create(virt_dev.virtmouse.virt_mouse_class, NULL,
				virt_dev.virtmouse.devno, NULL, "virtmouse");
        if(IS_ERR(dev)) {
                printk("create device failed \n");
                goto unreg_class;
        }

	return 0;

unreg_class:
	class_destroy(virt_dev.virtmouse.virt_mouse_class);
unreg_cdev:
	cdev_del(&(virt_dev.virtmouse.mousecdev));
unreg_region:
	unregister_chrdev_region(virt_dev.virtmouse.devno ,1);

	return -1;
}

static void unregister_virtmouse(void)
{
	device_destroy(virt_dev.virtmouse.virt_mouse_class, virt_dev.virtmouse.devno);
	class_destroy(virt_dev.virtmouse.virt_mouse_class);
	cdev_del(&(virt_dev.virtmouse.mousecdev));
	unregister_chrdev_region(virt_dev.virtmouse.devno, 1);
}

static int __init button_init(void)
{
	int error = -1;
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
	set_bit(EV_ABS, virt_dev.button_dev->evbit);
	set_bit(EV_REL, virt_dev.button_dev->evbit);

	/*actions for mouse*/
	set_bit(REL_X, virt_dev.button_dev->relbit);
	set_bit(REL_Y, virt_dev.button_dev->relbit);
	set_bit(BTN_LEFT, virt_dev.button_dev->keybit);
	set_bit(BTN_RIGHT, virt_dev.button_dev->keybit);
	set_bit(BTN_MIDDLE, virt_dev.button_dev->keybit);

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

	register_virtmouse();

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
	unregister_virtmouse();
}

module_init(button_init);
module_exit(button_exit);

MODULE_AUTHOR("LiangQiangqiang");
MODULE_DESCRIPTION("PKUNITY-3 SOC input driver");
MODULE_LICENSE("GPL v2");
