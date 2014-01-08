#include <linux/module.h>

static int __init joystick_init( void )
{
  printk( "hello world!\n" );
  return 0;
}

static void __exit joystick_exit( void )
{
  printk( "goodbye world!\n" );
}

module_init( joystick_init );
module_exit( joystick_exit );
