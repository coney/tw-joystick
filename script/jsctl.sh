#!/bin/bash

MODULE=tw-joystick
MODULE_FILE=${MODULE}.ko
MODULE_PATH="/lib/modules/`uname -r`/kernel/drivers/input/joystick/"

function load_driver {
    # temporarily setup gpio mode to pull down by gpio utility
    # for ((i=0;i<=20;i++)); do gpio mode $i up;done
    insmod $MODULE_FILE
}

function unload_driver {
    rmmod $MODULE
}

function reload_driver {
    unload_driver
    load_driver
}

function install_driver {
    mkdir -p $MODULE_PATH
    cp -f $MODULE_FILE $MODULE_PATH
}



case $1 in
    load)
        echo "loading $MODULE"
        load_driver;;

    unload)
        echo "unloading $MODULE"
        unload_driver;;
    
    reload)
        echo "reloading $MODULE"
        reload_driver;;
    install)
        echo "installing $MODULE into $MODULE_PATH"
        install_driver;;
esac
