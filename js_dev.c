#include <linux/pci.h>
#include <linux/input.h>
#include <linux/platform_device.h>

#include "js_log.h"
#include "js_dev.h"

static struct input_dev *js_input_dev = NULL;

void js_device_update(void)
{
    input_sync(js_input_dev);
}

static int js_device_open(struct input_dev *dev)
{
    logdebug("device opened!\n");

    return 0;
}

static void js_device_close(struct input_dev *dev)
{
    logdebug("device closed!\n");
}

static void js_device_register_capability(struct input_dev *dev)
{
    dev->name = "Thoughtworks Fake Joystick";
    //dev->phys;

    dev->open = js_device_open;
    dev->close = js_device_close;

    set_bit(EV_KEY, dev->evbit);
    set_bit(EV_ABS, dev->evbit);

    input_set_abs_params(dev, ABS_X, -1, 1, 0, 0);
    input_set_abs_params(dev, ABS_Y, -1, 1, 0, 0);


    set_bit(BTN_A, dev->keybit);
    set_bit(BTN_B, dev->keybit);
    set_bit(BTN_C, dev->keybit);
    set_bit(BTN_X, dev->keybit);
    set_bit(BTN_Y, dev->keybit);
    set_bit(BTN_Z, dev->keybit);
    set_bit(BTN_TL, dev->keybit);
    set_bit(BTN_TR, dev->keybit);

}

int js_device_init(void)
{
    int ret = 0;


    js_input_dev = input_allocate_device();
    if (!js_input_dev)
    {
        ret = -ENOMEM;
        logerror("failed to alloc joystick device!\n");
        goto error;
    }

    js_device_register_capability(js_input_dev);

    if ((input_register_device(js_input_dev)) != 0)
    {
        logerror("failed to register joystick device!\n");
        goto error;
    }

    return 0;

error:
    js_device_clear();
    return ret;
}

void js_device_clear(void)
{
    if (js_input_dev)
    {
        input_unregister_device(js_input_dev);
        input_free_device(js_input_dev);
        js_input_dev = NULL;
    }
}

static int process_direction_button(int button, int value)
{
    logdebug("report direction %u\n", button);
    switch (button)
    {
    case JSBTN_UP:
        input_report_abs(js_input_dev, ABS_Y, -1 * !!value);
        break;
    case JSBTN_DOWN:
        input_report_abs(js_input_dev, ABS_Y, 1 * !!value);
        break;
    case JSBTN_LEFT:
        input_report_abs(js_input_dev, ABS_X, -1 * !!value);
        break;
    case JSBTN_RIGHT:
        input_report_abs(js_input_dev, ABS_X, 1 * !!value);
        break;
    default:
        return -ENOENT;
    }
    input_sync(js_input_dev);
    return 0;
}

static int js_key_map[] = {
    KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED,
    BTN_A, BTN_B, BTN_C, BTN_X, BTN_Y, BTN_Z
};

static int process_key_button(int button, int value)
{
    if (button <= ARRAY_SIZE(js_key_map))
    {
        logdebug("report key %u\n", js_key_map[button]);
        input_report_key(js_input_dev, js_key_map[button], value);
        input_sync(js_input_dev);
        return 0;
    }
    else
    {
        return -ENOENT;
    }

}

int js_device_process(int id, int button, int value)
{
    if (JS_IS_DIRECTION(button))
    {
        return process_direction_button(button, value);
    }
    else if (JS_IS_BUTTON(button))
    {
        return process_key_button(button, value);
    }
    else
    {
        logwarn("unregisterd button %02X\n", button);
        return -ENOENT;
    }
}
