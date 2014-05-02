#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/moduleparam.h>
#include "js_log.h"
#include "js_input_gpio.h"
#include "js_dev.h"

#define JS_MAX_INPUT_GPIO_COUNT (JS_PLAYER_COUNT * JSBTN_COUNT)

typedef struct gpio_config
{
    unsigned int player;
    unsigned int button;
    unsigned int gpio;
    unsigned int irq;
    struct timer_list timer;
    char irq_name[16];
} gpio_config_t;
static gpio_config_t g_gpio_configs[JS_MAX_INPUT_GPIO_COUNT];


/**************************************************************************
+----------+-Rev2-+------+--------+------+-------+
| wiringPi | GPIO | Phys | Name   | Mode | Value |
+----------+------+------+--------+------+-------+
|      0   |  17  |  11  | GPIO 0 | IN   | Low   |
|      1   |  18  |  12  | GPIO 1 | IN   | Low   |
|      2   |  27  |  13  | GPIO 2 | IN   | Low   |
|      3   |  22  |  15  | GPIO 3 | IN   | Low   |
|      4   |  23  |  16  | GPIO 4 | IN   | Low   |
|      5   |  24  |  18  | GPIO 5 | IN   | Low   |
|      6   |  25  |  22  | GPIO 6 | IN   | Low   |
|      7   |   4  |   7  | GPIO 7 | IN   | Low   |
|      8   |   2  |   3  | SDA    | ALT0 | High  |
|      9   |   3  |   5  | SCL    | ALT0 | High  |
|     10   |   8  |  24  | CE0    | IN   | Low   |
|     11   |   7  |  26  | CE1    | IN   | Low   |
|     12   |  10  |  19  | MOSI   | IN   | Low   |
|     13   |   9  |  21  | MISO   | IN   | Low   |
|     14   |  11  |  23  | SCLK   | IN   | Low   |
|     15   |  14  |   8  | TxD    | ALT0 | High  |
|     16   |  15  |  10  | RxD    | ALT0 | High  |
|     17   |  28  |   3  | GPIO 8 | ALT2 | Low   |
|     18   |  29  |   4  | GPIO 9 | ALT2 | Low   |
|     19   |  30  |   5  | GPIO10 | ALT2 | Low   |
|     20   |  31  |   6  | GPIO11 | ALT2 | Low   |
+----------+------+------+--------+------+-------+
***************************************************************************/

static char *key_mapping = "(7,11,8,9,25,10,24,22),(17,18,27,23,28,29,30,31)";
module_param(key_mapping, charp, 0644);

static void gpio_timer_callback(unsigned long data)
{
    gpio_config_t *config = (gpio_config_t *)data;
    int value = !gpio_get_value(config->gpio);

    logdebug("index[%u] gpio[%02u]=%d\n",
        config->button, config->gpio, value);
    
    js_device_process(config->player, config->button, value);
}

static irqreturn_t gpio_irq_handler(int irq, void *data)
{
    gpio_config_t *config = (gpio_config_t *)data;
    mod_timer(&config->timer, jiffies + 5);
    return IRQ_HANDLED;
}

static int setup_gpio_irq(gpio_config_t *config)
{
    unsigned int gpio = config->gpio;
    BUG_ON(gpio == 0);

    gpio_direction_input(gpio);
    config->irq = gpio_to_irq(gpio);
    snprintf(config->irq_name, sizeof(config->irq_name), "gpio%02u", gpio);

    init_timer(&config->timer);
    config->timer.data = (unsigned long)config;
    config->timer.function = gpio_timer_callback;

    return request_irq(config->irq, gpio_irq_handler, 
        IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
        config->irq_name, config);
    return 0;
}

static void free_gpio_irq(gpio_config_t *config)
{
    BUG_ON(config->gpio == 0);
    del_timer(&config->timer);
    if (config->irq) {
        free_irq(config->irq, config);
    }
}

static int setup_gpio_config(const char *mapping, gpio_config_t *config) {
    // ugly but easy and robust
    unsigned int gpios[16] = {0};
    int count = sscanf(mapping, 
        "(%u,%u,%u,%u,%u,%u,%u,%u),(%u,%u,%u,%u,%u,%u,%u,%u)",
        gpios, gpios + 1, gpios + 2, gpios + 3, 
        gpios + 4, gpios + 5, gpios + 6, gpios + 7, 
        gpios + 8, gpios + 9, gpios + 10, gpios + 11, 
        gpios + 12, gpios + 13, gpios + 14, gpios + 15);

    if (count != ARRAY_SIZE(gpios)) {
        return -EINVAL;
    }
    

    unsigned int index = 0;
    for (unsigned int player = 0; player < 2; ++player) {
        for (unsigned int btn = JSBTN_UP; btn <= JSBTN_X; ++btn) {
            unsigned int gpio = gpios[index++];
            // skip some key configs
            if (gpio) {
                config->player = player;
                config->button = btn;
                config->gpio = gpio;
                logdebug("setup player:%u button:%u gpio:%u\n",
                    config->player, config->button, config->gpio);
                ++config;
            }
        }
    }

    return 0;
}

int js_input_gpio_init( void )
{
    int ret;

    memset(g_gpio_configs, 0, sizeof(g_gpio_configs));

    if ((ret = setup_gpio_config(key_mapping, g_gpio_configs))) {
        logerror("invalid key mapping '%s'\n", key_mapping);
        return ret;
    }


    for (unsigned int i = 0; i < JS_MAX_INPUT_GPIO_COUNT; ++i) {
        gpio_config_t *config = g_gpio_configs + i;
        if (!config->gpio) {
            break;
        }

        if ((ret = setup_gpio_irq(config))) {
            logerror("setup gpio[%u] failed! err=%d\n", config->gpio, ret);
            goto error;
        }
        logdebug("setup gpio[%u] irq[%u]\n", config->gpio, config->irq);
    }
     
    return 0;

error:
    js_input_gpio_clear();
    return ret;
}

void js_input_gpio_clear( void )
{
    unsigned int i;
    for (i = 0; i < JS_MAX_INPUT_GPIO_COUNT; ++i)
    {
        gpio_config_t *config = g_gpio_configs + i;
        if (!config->gpio) {
            break;
        }
        free_gpio_irq(config);
    }

    synchronize_rcu();
    memset(g_gpio_configs, 0, sizeof(g_gpio_configs));
}
