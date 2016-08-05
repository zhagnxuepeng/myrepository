#include <linux/module.h>  
#include <linux/init.h>  
#include <linux/workqueue.h>  
#include<linux/param.h> // for HZ
#include<linux/jiffies.h> 
static struct workqueue_struct *queue=NULL;  
static struct delayed_work   work;  
static void work_handler(struct work_struct *data)  
{  
	printk(KERN_INFO"work handler function\n");  
}  
static int  test_init(void)  
{  
	queue=create_singlethread_workqueue("hello world");
	if (!queue)  
		goto err;  
	INIT_DELAYED_WORK(&work,work_handler);  
	//	schedule_work(&work);  
  schedule_delayed_work(&work,msecs_to_jiffies(50));


	return 0;  
err:  
	return-1;  
}  

static   void  test_exit(void)  
{  
	destroy_workqueue(queue);  
}  
MODULE_LICENSE("GPL");  
module_init(test_init);  
module_exit(test_exit);  
