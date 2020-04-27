#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include<linux/linkage.h>
#include <linux/uaccess.h>

asmlinkage long sys_GETTIME(unsigned long __user *sec, unsigned long __user *nsec)
{
	struct timespec now;
	getnstimeofday(&now);

	copy_to_user(sec, &(now.tv_sec), sizeof(long));
	copy_to_user(nsec, &(now.tv_nsec), sizeof(long));
        return 0;
}
