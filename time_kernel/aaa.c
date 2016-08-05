

#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
struct timer_list timer;
void timer_handler(unsigned long data) {
	printk(KERN_INFO"timer pending:%d\n", timer_pending(&timer));
	mod_timer(&timer, jiffies+msecs_to_jiffies(1000));
	printk(KERN_INFO"jiffies:%ld, data:%ld\n", jiffies, data);
}
int timer_init(void) {
	printk(KERN_INFO"%s jiffies:%ld\n", __func__, jiffies);
	printk(KERN_INFO"ji:%d,HZ:%d\n", jiffies_to_msecs(250), HZ);
	init_timer(&timer);
	timer.data = 45;
	timer.function = timer_handler;
	timer.expires = jiffies + HZ;
	add_timer(&timer);
	printk(KERN_INFO"timer pending:%d\n", timer_pending(&timer));
	return 0;
}

void timer_exit(void) {
	printk(KERN_INFO"%s jiffies:%ld\n", __func__, jiffies);
	del_timer(&timer);
}


module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
