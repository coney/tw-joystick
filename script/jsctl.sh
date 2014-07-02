#!/bin/bash

MODULE=tw-joystick
MODULE_FILE=${MODULE}.ko
DSTPATH="/lib/modules/`uname -r`/kernel/drivers/input/joystick/"

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
    mkdir -p $DSTPATH
    cp -f $MODULE_FILE $DSTPATH
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
        echo "installing $MODULE into $DSTPATH"
        install_driver;;
esac
