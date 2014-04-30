#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include "js_log.h"
#include "js_input_gpio.h"
#include "js_dev.h"

#define JS_MAX_INPUT_GPIO_COUNT (JS_PLAYER_COUNT * JSBTN_COUNT)

typedef struct gpio_config
{
    unsigned int button;
    unsigned int gpio;
    unsigned int irq;
    struct timer_list timer;
    char irq_name[16];
} gpio_config_t;


struct {
    unsigned int player;
    unsigned int gpio;
    unsigned int button;
} g_gpio_mapping[JS_PLAYER_COUNT][JSBTN_COUNT];

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

static gpio_config_t g_gpio_configs[JS_MAX_INPUT_GPIO_COUNT];
static const char *key_mapping = "(7,11,8,9,25,10,24,22),(17,18,27,23,28,29,30,31)";
module_param(key_mapping, charp, S_IRUGO);

static void gpio_timer_callback(unsigned long data)
{
    gpio_config_t *config = (gpio_config_t *)data;
    int value = !gpio_get_value(config->gpio);

    logdebug("index[%u] gpio[%02u]=%d\n",
        config->button, config->gpio, value);
    
    js_device_process(0, config->button, value);
}

static irqreturn_t gpio_irq_handler(int irq, void *data)
{
    gpio_config_t *config = (gpio_config_t *)data;
    mod_timer(&config->timer, jiffies + 5);
    return IRQ_HANDLED;
}

static int setup_gpio_irq(gpio_config_t *config, unsigned int gpio,
                          unsigned int button)
{
    BUG_ON(gpio == 0);

    gpio_direction_input(gpio);

    config->button = button;
    config->gpio = gpio;
    config->irq = gpio_to_irq(gpio);
    snprintf(config->irq_name, sizeof(config->irq_name), "gpio%02u", gpio);

    init_timer(&config->timer);
    config->timer.data = (unsigned long)config;
    config->timer.function = gpio_timer_callback;

    return request_irq(config->irq, gpio_irq_handler, 
        IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
        config->irq_name, config);
}

static void free_gpio_irq(gpio_config_t *config)
{
    // pass uninitialized gpio config
    if (config->gpio)
    {
        free_irq(config->irq, config);
        del_timer(&config->timer);
    }
}

static int parse_gpio_mapping(const char *mapping) {
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
            g_gpio_mapping[player][btn].player = player;
            g_gpio_mapping[player][btn].button = btn;
            g_gpio_mapping[player][btn].gpio = gpios[index++];
        }
        
    }

    return 0;
}

int js_input_gpio_init( void )
{
    int ret;
    unsigned int i;

    //BUG_ON(ARRAY_SIZE(g_gpio_mapping) > JS_MAX_INPUT_GPIO_COUNT);

    memset(g_gpio_configs, 0, sizeof(g_gpio_configs));
    memset(g_gpio_mapping, 0, sizeof(g_gpio_mapping));

    if ((ret = parse_gpio_mapping(key_mapping))) {
        logerror("invalid key mapping '%s'\n", key_mapping);
        return ret;
    }
    
    return -EINVAL;

    //for (i = 0; i < ARRAY_SIZE(g_gpio_mapping); ++i)
    //{
    //    gpio_config_t *config = g_gpio_configs +i;

    //    if ((ret = setup_gpio_irq(config, 
    //        g_gpio_mapping[i].gpio,
    //        g_gpio_mapping[i].button)))
    //    {
    //        logerror("setup gpio[%u] failed!err=%d\n",
    //            g_gpio_mapping[i].gpio, ret);
    //        js_input_gpio_clear();
    //        return ret;
    //    }
    //    logdebug("setup gpio[%02u] irq[%u]\n", config->gpio, config->irq);
    //}

    return 0;
}

void js_input_gpio_clear( void )
{
    unsigned int i;
    for (i = 0; i < ARRAY_SIZE(g_gpio_mapping); ++i)
    {
        free_gpio_irq(g_gpio_configs + i);
    }

    synchronize_rcu();
    memset(g_gpio_configs, 0, sizeof(g_gpio_configs));
}
