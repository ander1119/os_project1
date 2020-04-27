#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>

asmlinkage long sys_PRINTK(char __user *buf, int len)
{
	char msg[256];
	copy_from_user(msg, buf, len);
	printk("%s\n", msg);
        return 0;
}
