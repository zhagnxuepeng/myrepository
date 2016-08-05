/*
 *chrdrvtest字符驱动程序
 * */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h> //kmalloc函数的头文件void * kmalloc(size_t size, int flags);
#include <linux/init.h>
#include <linux/fs.h> 	//包含file结构体
#include <linux/cdev.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/timer.h>

#include <asm/uaccess.h>

#include"chrdrvtest.h" 	//自定义头文件

#define N 128
#define NUM_OF_DEV 1   	//设备的个数

typedef struct cha_device { //定义一个字符设备结构体
	struct cdev cdev; 		//cdev 结构体 
	char data[N];   		//字符设备总共128字节
	spinlock_t lock;  		//spinlock_t类型的标记，控制对字符设备的读和写
}char_mem;

int MAJOR = 290;//主设备号
int MINOR = 0; 	//次设备号
dev_t dev_no;  	//设备号
int fd;      	//定义全局文件描述符
char_mem char_dev;
char_mem *p;
struct class *cls; 
//struct timer_list TIMER_INITIALIZER(func_timer, 200, );

static int open_chr(struct inode *inode, struct file *file)
{
	printk("****open_chr**** \n");
	return 0;
}

static ssize_t write_chr(struct file *file, const char __user *user, size_t size, loff_t *loff)
{
	int sz = size;
	ssize_t ret = 0;

	printk("****write_chr**** \n");

	if (copy_from_user (p->data, user, sz)) {
		printk("copy_from_user 错误! \n");
		ret = -EFAULT;
	}
	else {
		printk("The mesage from user space: %s \n", p->data);
	}
	return ret;
}

static ssize_t read_chr(struct file *file, char __user *user, size_t size, loff_t *loff)
{
	copy_to_user(user, &p->data, N);
	printk("****read_chr**** \n");
	return 0;
}

static long ioctl_chr(struct file* file, unsigned int cmd, unsigned long arg)
{
	printk("****ioctl_chr**** \n");

	switch(cmd) {
	case A:

		printk("**command AAAAAAAAAAA \n");
		break;
	case B:
		printk("**command BBBBBBBBBBB \n");
		break;
	case C:
		printk("**command CCCCCCCCCCC \n");
		break;
	case D:
		printk("**command DDDDDDDDDDD \n");
		break;
	case E:
		printk("**command EEEEEEEEEEE \n");
		break;
	default:
		break;
	}
	return 0;
}

static int release_chr(struct inode *inode, struct file *file)
{
	printk("****release_chr**** \n");
	return 0;
}

static struct file_operations chrdev_fops = {
	.owner = THIS_MODULE,
	.open = open_chr,
	.write = write_chr,
	.read = read_chr,
	.unlocked_ioctl = ioctl_chr,
	.release = release_chr,
};

static int chrdrvtest_init(void)
{
	int ERROR_NO; 	//定义一个错误码
	int ret;
	int err;

	printk("******enter chrdrvtest_init****** \n"); 	//进入初始化函数

	dev_no = MKDEV(MAJOR,MINOR); //申请设备号

	ret = register_chrdev_region(dev_no,NUM_OF_DEV,"memval"); 	//注册设备号
	if(ret != 0) {
		return -EBUSY;
	}

	p = kmalloc(sizeof (char_mem), GFP_KERNEL); 	//申请自定义结构体cha_device的内存空间
	if (!p) {
		printk("***ERROR,ERROR,kmalloc() execute ERROR!*** \n");
		ERROR_NO = -ENOMEM;
		goto fail_kmalloc;
	}

	memset(p,65,sizeof(char_mem)); 	//初始化申请的内存空间为0

	printk("val:%s\n",p->data);

	cdev_init(&p->cdev,&chrdev_fops); 	//初始化设备
	err = cdev_add(&p->cdev,dev_no,NUM_OF_DEV); 	//添加设备
	if(err) {
		printk("***添加设备失败cdev_add***");
		goto fail_cdev_add;
	}
	cls = class_create(THIS_MODULE, "bsp"); 	//自动创建设备节点
	device_create(cls, NULL, dev_no, NULL, "memzone");

	return 0;

fail_cdev_add:
fail_kmalloc:
	unregister_chrdev_region(dev_no, NUM_OF_DEV);
	return ERROR_NO;
}

static void chrdrvtest_exit(void)
{
	device_destroy(cls,dev_no);
	class_destroy(cls);
	cdev_del(&p->cdev); 	//注销cdev结构体
	kfree(p); 	//释放申请的结构体内存
	unregister_chrdev_region(dev_no,NUM_OF_DEV);  	//注销申请的设备
	printk("******exit chrdrvtest_exit****** \n");
}

module_init(chrdrvtest_init);
module_exit(chrdrvtest_exit);

MODULE_LICENSE("GPL");
