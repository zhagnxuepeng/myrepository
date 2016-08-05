#include <linux/kthread.h>  
#include <linux/module.h> 
#include<linux/slab.h>
#include<linux/timer.h> // for timer_list API
#include<linux/param.h> // for HZ
#include<linux/jiffies.h> // for jiffies
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
struct timer_list timer;

static struct task_struct * MyThread = NULL; 

void timer_handler(unsigned long data) {
		mod_timer(&timer, jiffies+msecs_to_jiffies(50));
		printk("hello world \n");
	
}
static int MyPrintk(void *data)  
{ 

	init_timer(&timer);
	timer.data = 45;
	timer.function = timer_handler;
	timer.expires = jiffies + HZ;
	add_timer(&timer);
	return 0;  

}  
static int __init init_kthread(void)  
{  
	MyThread = kthread_run(MyPrintk,NULL,"mythread");  
	return 0;  
}  
static void __exit exit_kthread(void)  
{
	if(MyThread)
	kthread_stop(MyThread);
	printk("tui chu\n ");
}  
module_init(init_kthread);  
module_exit(exit_kthread);  
