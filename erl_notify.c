/* Standard in kernel modules */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>



struct erl_device {
        struct kobject kobj;
        unsigned int value;
};

static struct erl_device ErlDevice;




static ssize_t sys_trigger_get(struct kobject *kobj, struct kobj_attribute *attr,
                char *buf)
{
        struct erl_device *erl;
        erl = container_of(kobj, struct erl_device, kobj);
        return sprintf(buf, "%d\n", erl->value);
}

static ssize_t sys_trigger_set(struct kobject *kobj, struct kobj_attribute *attr,
                      const char *buf, size_t count)
{
        struct erl_device *erl;
        unsigned int val = 0;
        erl = container_of(kobj, struct erl_device, kobj);
        sscanf(buf, "%du", &val);
        erl->value = val;
	printk(KERN_INFO "erl_notify = %d", val);
	sysfs_notify(kobj, NULL, "trigger");
        return count;
}

static struct kobj_attribute sys_trigger_attr =__ATTR(trigger, 0660,
                sys_trigger_get, sys_trigger_set);




static struct attribute *erl_attrs[] = {
        &sys_trigger_attr.attr,
        NULL
};

static struct kobj_type erl_ktype = {
        .sysfs_ops = &kobj_sysfs_ops,
        .default_attrs = erl_attrs,
};




static int __init erl_init(void)
{
	ErlDevice.value = 42;
        if(kobject_init_and_add(&ErlDevice.kobj,&erl_ktype,kernel_kobj,"erl_notify")) {
                printk(KERN_ERR "failed to init erl_notify sysfs");
                return 0;
        }

	printk(KERN_INFO "erl_notify driver is loaded");
        return 0;
}

void erl_exit(void)
{
        kobject_put(&ErlDevice.kobj);
        printk(KERN_INFO "erl_notify driver is unloaded");
}

module_init(erl_init);
module_exit(erl_exit);

MODULE_AUTHOR("Max Lapshin <max@maxidoors.ru>");
MODULE_DESCRIPTION("Erlang sysfs polling example");
MODULE_LICENSE("Dual BSD/GPL");

