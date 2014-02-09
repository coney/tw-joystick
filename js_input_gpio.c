#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include "js_log.h"
#include "js_input_gpio.h"
#include "js_dev.h"

#define JS_MAX_INPUT_GPIO_COUNT 16

typedef struct gpio_config
{
    unsigned int button_index;
    unsigned int gpio;
    unsigned int irq;
    struct timer_list timer;
    char irq_name[16];
} gpio_config_t;


struct {
    unsigned int gpio;
    unsigned int button;
} g_gpio_mapping[] = {
    {7,     JSBTN_UP},
    {11,    JSBTN_DOWN}, 
    {8,     JSBTN_LEFT}, 
    {9,     JSBTN_RIGHT},
    {25,    JSBTN_A},
    {10,    JSBTN_B},
    {24,    JSBTN_C},
    {22,    JSBTN_X}
};

static gpio_config_t g_gpio_configs[JS_MAX_INPUT_GPIO_COUNT];

static void gpio_timer_callback(unsigned long data)
{
    gpio_config_t *config = (gpio_config_t *)data;
    logdebug("index[%u] gpio[%02u]=%d\n",
        config->button_index, config->gpio, gpio_get_value(config->gpio));
}

static irqreturn_t gpio_irq_handler(int irq, void *data)
{
    gpio_config_t *config = (gpio_config_t *)data;
    mod_timer(&config->timer, jiffies + 5);
    return IRQ_HANDLED;
}

static int setup_gpio_irq(gpio_config_t *config, unsigned int gpio,
                          unsigned int button_index)
{
    BUG_ON(gpio == 0);
    config->button_index = button_index;
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

int js_input_gpio_init( void )
{
    int ret;
    unsigned int i;

    BUG_ON(ARRAY_SIZE(g_gpio_mapping) > JS_MAX_INPUT_GPIO_COUNT);

    memset(g_gpio_configs, 0, sizeof(g_gpio_configs));
    for (i = 0; i < ARRAY_SIZE(g_gpio_mapping); ++i)
    {
        gpio_config_t *config = g_gpio_configs +i;

        if ((ret = setup_gpio_irq(config, 
            g_gpio_mapping[i].gpio,
            g_gpio_mapping[i].button)))
        {
            logerror("setup gpio[%u] failed!err=%d\n",
                g_gpio_mapping[i].gpio, ret);
            js_input_gpio_clear();
            return ret;
        }
        logdebug("setup gpio[%02u] irq[%u]\n", config->gpio, config->irq);
    }

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
