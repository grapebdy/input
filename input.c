#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/input.h>
#include <linux/bitops.h>
#include <linux/interrupt.h>

#define  BUTTON_IRQ 0xa
static struct input_dev *button_dev;

//static void button_interrupt(int irq, void*dummy, struct pt_regs *fp)
static irqreturn_t button_interrupt(int irq,void *dev_instance)
{
	input_report_key(button_dev, KEY_1, 1);
	input_sync(button_dev);
	return IRQ_RETVAL(1);
}

static int __init button_init(void)
{
	int error;
	if (request_irq(BUTTON_IRQ, button_interrupt, IRQF_SHARED, "button", NULL)) {
		printk(KERN_ERR"button.c: Can't allocate irq %d\n", BUTTON_IRQ);
		return -EBUSY;
	}

	button_dev = input_allocate_device();
	if (!button_dev) {
		printk(KERN_ERR"button.c: Not enough memory\n");
		error = -ENOMEM;
		goto err_free_irq;
	}

	button_dev->evbit[0] = BIT(EV_KEY);
	button_dev->keybit[KEY_1] = BIT(KEY_1);

	error = input_register_device(button_dev);
	if (error) {
		printk(KERN_ERR"button.c: Failed to register device\n");
		goto err_free_dev;
	}

	return 0;

err_free_dev:
	input_free_device(button_dev);
err_free_irq:
	free_irq(BUTTON_IRQ, button_interrupt);
	return error;
}

static void __exit button_exit(void)
{
	input_unregister_device(button_dev);
	free_irq(BUTTON_IRQ, button_interrupt);
}

module_init(button_init);
module_exit(button_exit);
