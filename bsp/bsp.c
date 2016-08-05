#include <linux/init.h>//module_init  module_exit
#include <linux/kernel.h>//printk
#include <linux/fs.h>//register_chrdev_region  unregister_chrdev_region
#include <linux/cdev.h>//cdev_init  cdev_add  cdev_del
#include <asm/uaccess.h>
#include <linux/sched.h>
#include<linux/device.h>
#include<linux/jiffies.h> // for jiffies
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include<linux/param.h> 
#include <linux/kthread.h>  
#include <linux/workqueue.h> 
#include <linux/string.h>
#include "bsp.h"
MODULE_LICENSE("GPL");
#define  MARJOR    290
#define  MINFOR    0
#define  NUM_OF_DEVICE  1
dev_t  devno;
struct  cdev  cdev;
char  dataa[1024];
struct class *cls;
wait_queue_head_t  hello_readq;
int flag  = 0;
int flags =0;
spinlock_t memlock;
struct timer_list timer;
static struct task_struct * MyThreadA = NULL; 
static struct task_struct * MyThreadB = NULL; 
static struct workqueue_struct *queue=NULL;  
static struct delayed_work   work;  
static    int chrdev_open(struct inode *inode, struct file *file)
{
	printk("chrdev_open  \n");
	return   0;
}
static  ssize_t chrdev_read  (struct file *file, char __user *buf, size_t  size, loff_t *loff)
{
	int  ret;
	if(size  >  1024)
		size  = 1024;
	if(size  <  0)
		return -ENOMEM;
	wait_event_interruptible(hello_readq,flag != 0);
	ret  =  copy_to_user(buf,dataa,size);
	if(0  != ret)
		return  -EINVAL;
	flag = 0;
	printk("chrdev_read   \n");
	return   size;
}
static  ssize_t chrdev_write  (struct file *file, const char __user *buf, size_t  size, loff_t *loff)
{

	int  ret;
	if(size  >  1024)
		size  = 1024;
	if(size  <  0)
		return  -ENOMEM;
	memset(dataa,0,sizeof(dataa));
	ret = copy_from_user(dataa,buf,size);
	if(0  != ret)
		return  -EINVAL;
	flag  = 1;
	wake_up_interruptible(&hello_readq);
	printk("chrdev_write  \n");
	return  size;
}
static void timer_handler(unsigned long data) {
	
	printk("timer:%s \n",dataa);
	if(1 == flags)
	{
		spin_lock(&memlock);
		strcat(dataa, "hello world");
		flags =0;
		spin_unlock(&memlock);
	}
	mod_timer(&timer, jiffies+msecs_to_jiffies(1000));


}
static int MyPrintkA(void *data)  
{ 
	init_timer(&timer);
	timer.data = 45;
	timer.function = timer_handler;
	timer.expires = jiffies + 5*HZ;
	add_timer(&timer);
	return 0;  
}  
static void work_handler(struct work_struct *data)  
{  
	spin_lock(&memlock);
	printk(KERN_INFO "work handler function "); 
	printk("work:%s\n",dataa);
	spin_unlock(&memlock);
	queue_delayed_work(queue,&work,msecs_to_jiffies(5000));
	
}  

static int MyPrintkB(void *data)  
{ 

	queue=create_singlethread_workqueue("hello world");
	if (!queue)  
		return -1;  
	INIT_DELAYED_WORK(&work,work_handler);  
	queue_delayed_work(queue,&work,msecs_to_jiffies(5000));

	return 0;  
}  

long chrdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

	switch(cmd) {
	case A:
		MyThreadA = kthread_run(MyPrintkA,NULL,"mythreadA");  
		printk("**command AAAAAAAAAAA \n");
		break;
	case B:
		MyThreadB = kthread_run(MyPrintkB,NULL,"mythreadB");  
		printk("**command BBBBBBBBBBB \n");
		break;
	case C:
		flags = 1;
		printk("**command CCCCCCCCCCC \n");
		break;
	default:
		break;
	}
	return 0;
}
struct  file_operations   chrdev_fops  = {
	.owner  = 	THIS_MODULE,
	.open   =  chrdev_open,
	.read   =  chrdev_read,
	.write  = chrdev_write,
	.unlocked_ioctl = chrdev_ioctl,
};
//用户定义的加载函数
int  chrdev_init(void)
{
	int ret;
	//申请设备号
	devno  = MKDEV(MARJOR,MINFOR);
	spin_lock_init(&memlock);
	//注册设备号
	ret  = register_chrdev_region(devno,NUM_OF_DEVICE,"chrdev");
	if(0!= ret)
	{
		printk("register_chrdev_region  fail\n");
		return -EBUSY;
	}
	//初始化cdev结构体
	cdev_init(&cdev,&chrdev_fops);
	//将cdev添加到系统中
	cdev_add(&cdev,devno,NUM_OF_DEVICE);
	init_waitqueue_head(&hello_readq);
	cls = class_create(THIS_MODULE,"aaaa");
	device_create(cls,NULL,devno,NULL,"zhangxp");
	printk("chrdev_init   \n");
	return  0;
}
//用户定义卸载函数
void  chrdev_exit(void)
{
	cancel_delayed_work(&work);
	destroy_workqueue(queue);
	del_timer(&timer);

	device_destroy(cls,devno);
	class_destroy(cls);
	cdev_del(&cdev);
	unregister_chrdev_region(devno,1);
	printk("chrdev_exit \n");
}

//将系统实现的加载函数和用户定义的加载函数关联在一起
module_init(chrdev_init);
module_exit(chrdev_exit);

