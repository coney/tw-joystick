#include <linux/module.h>

#include "js_log.h"
#include "js_proc.h"
#include "js_dev.h"
#include "js_input_gpio.h"

MODULE_AUTHOR("Coney Wu <kunwu@thoughtworks.com>");
MODULE_LICENSE("GPL");

int __init joystick_init(void)
{
    int ret = 0;
    loginfo("load joystick driver!\n");

    if ((ret = js_device_init()) != 0) {
        logerror("failed to create input devices!\n");
        goto error;
    }

    if ((ret = js_proc_init()) != 0) {
        logerror("failed to create proc entry!\n");
        goto error;
    }

#ifdef RASPI
    loginfo("initalizing gpio\n");
    if ((ret = js_input_gpio_init() != 0))
    {
        logerror("failed to init gpio!\n");
        goto error;
    }
#endif // RASPI

    loginfo("joystick driver loaded!\n");
    return 0;

error:
#ifdef RASPI
    js_input_gpio_clear();
#endif // RASPI

    js_proc_clear();
    js_device_clear();

    return ret;
}

void __exit joystick_exit(void)
{
    loginfo("unload joystick driver!\n");

#ifdef RASPI
    js_input_gpio_clear();
#endif // RASPI

    js_proc_clear();

    js_device_clear();

    return;
}



module_init(joystick_init);
module_exit(joystick_exit);