#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>

static int hello_major       = 250;
static int hello_minor       = 0;
static int number_of_devices = 1;

static int flag = 0;

struct hello_device
{
	char data[128];
	spinlock_t lock;
	struct cdev cdev;
}hello_device;

static int hello_open (struct inode *inode, struct file *file)
{
	spin_lock(&hello_device.lock);//自旋锁锁住flag变量
	if (flag) {
		spin_unlock(&hello_device.lock);
		return -EBUSY;
	}
	flag++;
	spin_unlock(&hello_device.lock);//解锁
	
	printk (KERN_INFO "Hey! device opened\n");
	return 0;
}

static int hello_release (struct inode *inode, struct file *file)
{
	spin_lock(&hello_device.lock);//字符设备被关闭时，释放flag
	flag--;
	spin_unlock(&hello_device.lock);//解锁

	printk (KERN_INFO "Hmmm... device closed\n");
	return 0;
}

ssize_t hello_read (struct file *filp, char *buff, size_t count, loff_t *offp)
{
	ssize_t result = 0;

	if (count < 0) 
		return -EINVAL;
	if (count > 127) 
		count = 127;

	if (copy_to_user (buff, hello_device.data, count)) {
		result = -EFAULT;
	}
	else {
		printk (KERN_INFO "wrote %d bytes\n", count);
		result = count;
	}

	return result;
}

ssize_t hello_write (struct file *filp, const char  *buf, size_t count, loff_t *f_pos)
{
	ssize_t ret = 0;

	printk (KERN_INFO "Writing %d bytes\n", count);
	if (count > 127) return -ENOMEM;
	if (count < 0) return -EINVAL;
	if (copy_from_user (hello_device.data, buf, count)) {
		ret = -EFAULT;
	}
	else {
		hello_device.data[count] = '\0';
		printk (KERN_INFO"Received: %s\n", hello_device.data);
		ret = count;
	}

	return ret;
}

struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.open  = hello_open,
	.release = hello_release,
	.read  = hello_read,
	.write = hello_write
};

static void char_reg_setup_cdev (void)
{
	int error;
	dev_t devno;

	devno = MKDEV (hello_major, hello_minor);
	cdev_init (&hello_device.cdev, &hello_fops);
	hello_device.cdev.owner = THIS_MODULE;
	error = cdev_add (&hello_device.cdev, devno , 1);
	if (error)
		printk (KERN_NOTICE "Error %d adding char_reg_setup_cdev", error);
}

static int __init hello_2_init (void)
{
	int result;
	dev_t devno;

	devno = MKDEV (hello_major, hello_minor);
	result = register_chrdev_region (devno, number_of_devices, "hello");

	if (result < 0) {
		printk (KERN_WARNING "hello: can't get major number %d\n", hello_major);
		return result;
	}

	char_reg_setup_cdev ();
	spin_lock_init(&hello_device.lock);
	
	printk (KERN_INFO "char device registered\n");
	return 0;
}

static void __exit hello_2_exit (void)
{
	dev_t devno = MKDEV (hello_major, hello_minor);
	cdev_del (&hello_device.cdev);
	unregister_chrdev_region (devno, number_of_devices);
}

module_init (hello_2_init);
module_exit (hello_2_exit);

MODULE_LICENSE ("GPL");
