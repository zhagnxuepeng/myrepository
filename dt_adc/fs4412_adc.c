
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");


#define  ADC_MARJOR  500
#define  ADC_MINOR   0


struct  class  *  cls;
dev_t  devno;
struct  platform_device  *mydev;
int  key;
wait_queue_head_t   adc_readq;
int  flag  = 0;
struct  resource   * res_mem;
struct  resource  * res_irq;
void  __iomem  *  adc_base;
static ssize_t fs4412_adc_read (struct file *file, char __user *buf, size_t  size, loff_t *loff)
{


	int  data  = 0;
	writel(0x3,adc_base  + 0x1c);
	writel(1 <<  16|1 <<14|0xff  <<6 |0x1 <<0,adc_base  +0x00);

	wait_event_interruptible(adc_readq,flag  != 0);

	data  = readl(adc_base  + 0x0c)  &0xfff;
	if(copy_to_user(buf,&data,size))
		return  -EINVAL;


	flag  = 0;

	return  size;

}
struct  file_operations    fs4412_adc_fops  = {

	.owner  = THIS_MODULE,
	.read  =  fs4412_adc_read,

};
irqreturn_t  adc_handler(int  irqnum,void  *  dev)
{

	writel(0x0,adc_base  + 0x18);
	flag  = 1;
	wake_up_interruptible(&adc_readq);


	return  IRQ_HANDLED;


}
static int fs4412_adc_probe(struct platform_device *pdev)
{

#if  1

	int ret  =-ENOMEM;
	mydev  = pdev;
	devno  = MKDEV(ADC_MARJOR,ADC_MINOR);

	if(register_chrdev(ADC_MARJOR,"adc",&fs4412_adc_fops))
		goto  err1;




	res_mem  = platform_get_resource(pdev,IORESOURCE_MEM,0);
	if(NULL  == res_mem)
	{
	
		printk("platform_get_resource  res_mem  fail  \n");
		goto  err2;
	}
	res_irq  =  platform_get_resource(pdev,IORESOURCE_IRQ,0);
	if(NULL  == res_irq)
	{
		printk("platform_get_resource  res_irq  fail  \n");
		goto  err2;
	
	}


	adc_base  = ioremap(res_mem->start,res_mem->end  - res_mem->start);
	if(NULL  == adc_base)
	{

		printk("ioremap  fail  \n");
		goto  err2;

	}

	ret  = request_irq(res_irq->start,adc_handler,IRQF_DISABLED,"adc",NULL);
    if(0  != ret)
	{
	
		printk("request_irq  resource  0  fail  \n");
		goto  err3;
	}


	init_waitqueue_head(&adc_readq);
	cls  =  class_create(THIS_MODULE,"fs4412");
	device_create(cls,NULL,devno,NULL,"adc");
#endif
	printk("match  ok\n");
	return  0;
err3:
	iounmap(adc_base);
err2:
	unregister_chrdev(ADC_MARJOR,"adc");

#if  1
err1:
	return  ret;
#endif
}
static int fs4412_adc_remove(struct platform_device *pdev)
{



	device_destroy(cls,devno);
	class_destroy(cls);
	iounmap(adc_base);
	free_irq(res_irq->start,NULL);
	unregister_chrdev(ADC_MARJOR,"adc");

	printk("fs4412_adc_remove  \n");
	return   0;
}

struct  of_device_id  fs4412_adc_table[]  =  {

	{
	
		.compatible  = "fs4412,adc",
	
	},


};
struct  platform_driver   prv  = {


	.driver  = {
	
		.name  = "fs4412-adc",
		.of_match_table  =fs4412_adc_table ,
	
	},
	.probe   =  fs4412_adc_probe,
	.remove  =  fs4412_adc_remove,


}; 

int  fs4412_adc_init(void)
{

	platform_driver_register(&prv);
	printk("fs4412_adc_init  \n");
	return   0;
}

void  fs4412_adc_exit(void)
{

	platform_driver_unregister(&prv);
	printk("fs4412_adc_exit  \n");
}

module_init(fs4412_adc_init);
module_exit(fs4412_adc_exit);



