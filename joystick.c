#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include "js_log.h"
#include "js_proc.h"

MODULE_AUTHOR("Coney Wu <kunwu@thoughtworks.com>");
MODULE_LICENSE("GPL");

struct input_dev *vms_input_dev;
static struct platform_device *vms_dev;	/* Device structure */
/*
 * Sysfs method to input simulated coordinates to the virtual mouse driver
 */
static ssize_t
write_vms(struct device *dev,
struct device_attribute *attr, const char *buffer, size_t count)
{
    int x, y;
    sscanf(buffer, "%d%d", &x, &y);
    input_report_rel(vms_input_dev, REL_X, x);
    input_report_rel(vms_input_dev, REL_Y, y);
    input_sync(vms_input_dev);
    return count;
}

/* Attach the sysfs write method */ DEVICE_ATTR(coordinates, 0644, NULL,
    write_vms);
/* Attribute Descriptor */
static struct attribute *vms_attrs[] = {
    &dev_attr_coordinates.attr,
    NULL
};

/* Attribute group */
static struct attribute_group vms_attr_group = {
    .attrs = vms_attrs,
};

/* Driver Initialization */ int __init
joystick_init(void)
{
    loginfo("load joystick driver!\n");

    /* Register a platform device */
    vms_dev = platform_device_register_simple("vms", -1, NULL, 0);
    if (IS_ERR(vms_dev))
    {
        PTR_ERR(vms_dev);
        printk("vms_init: error\n");
    }
    /* Create a sysfs node to read simulated coordinates */
    sysfs_create_group(&vms_dev->dev.kobj, &vms_attr_group);
    /* Allocate an input device data structure */ vms_input_dev =
        input_allocate_device();
    if (!vms_input_dev)
    {
        printk("Bad input_alloc_device()\n");
    }
    /* Announce that the virtual mouse will generate relative coordinates */
    set_bit(EV_REL, vms_input_dev->evbit);
    set_bit(REL_X, vms_input_dev->relbit);
    set_bit(REL_Y, vms_input_dev->relbit);
    /* Register with the input subsystem */
    input_register_device(vms_input_dev);

    js_init_proc_entry();

    loginfo("joystick driver loaded!\n");

    return 0;
}

void __exit
joystick_exit(void)
{
    loginfo("unload joystick driver!\n");

    js_clear_proc_entry();

    /* Unregister from the input subsystem */
    input_unregister_device(vms_input_dev);

    /* Cleanup sysfs node */
    sysfs_remove_group(&vms_dev->dev.kobj, &vms_attr_group);

    /* Unregister driver */
    platform_device_unregister(vms_dev);

    return;
}



module_init(joystick_init);
module_exit(joystick_exit);