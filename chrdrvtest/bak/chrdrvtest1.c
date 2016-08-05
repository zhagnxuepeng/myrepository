#include<linux/init.h>
#include<linux/module.h>


static int chrdrvtest_init(void)
{
	printk("******enter chrdrvtest_init******\n");
	return 0;
}

static void chrdrvtest_exit(void)
{
	printk("******exit chrdrvtest_exit******\n");

}
module_init(chrdrvtest_init);
module_exit(chrdrvtest_exit);
MODULE_LICENSE("GPL");
