#include <linux/init.h>//module_init  module_exit
#include <linux/kernel.h>//printk
#include <linux/fs.h>//register_chrdev_region  unregister_chrdev_region
#include <linux/cdev.h>//cdev_init  cdev_add  cdev_del
#include <asm/uaccess.h>
#include <linux/sched.h>


#define  MARJOR    290
#define  MINFOR    0
#define  NUM_OF_DEVICE  1

dev_t  devno;
struct  cdev  cdev;
char  data[128] ;
wait_queue_head_t  hello_readq;
int flag  = 0;

static    int chrdev_open(struct inode *inode, struct file *file)
{

	printk("chrdev_open  \n");
	return   0;


}

static  ssize_t chrdev_read  (struct file *file, char __user *buf, size_t  size, loff_t *loff)
{

	int  ret;

	if(size  >  128)
		size  = 128;
	if(size  <  0)
		return -ENOMEM;
	wait_event_interruptible(hello_readq,flag != 0);
	ret  =  copy_to_user(buf,data,size);
	if(0  != ret)
		return  -EINVAL;

	flag = 0;

	printk("chrdev_read   \n");

	return   size;

}
static  	ssize_t chrdev_write  (struct file *file, const char __user *buf, size_t  size, loff_t *loff)
{

	int  ret;

	if(size  >  128)
		size  = 128;
	if(size  <  0)
		return  -ENOMEM;

	memset(data,0,sizeof(data));
	ret  = copy_from_user(data,buf,size);
	if(0  != ret)
		return  -EINVAL;


	flag  = 1;
	wake_up_interruptible(&hello_readq);

	printk("chrdev_write  \n");
	return  size;


}
struct  file_operations   chrdev_fops  = {


	.open   =  chrdev_open,
	.read   =  chrdev_read,
	.write  = chrdev_write,

};



//用户定义的加载函数
int  chrdev_init(void)
{

	
	int ret;
	//申请设备号
	devno  = MKDEV(MARJOR,MINFOR);


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

	printk("chrdev_init   \n");
	return  0;

}

//用户定义卸载函数
void  chrdev_exit(void)
{


	cdev_del(&cdev);
	unregister_chrdev_region(devno,1);

	printk("chrdev_exit   \n");
}


//将系统实现的加载函数和用户定义的加载函数关联在一起
module_init(chrdev_init);
module_exit(chrdev_exit);

