#!/bin/bash

MODULE=tw-joystick
MODULE_FILE=${MODULE}.ko

function load_driver {
    # temporarily setup gpio mode to pull down by gpio utility
    for ((i=0;i<=20;i++)); do gpio mode $i down;done
    insmod $MODULE_FILE
}

function unload_driver {
    rmmod $MODULE
}

function reload_driver {
    unload_driver
    load_driver
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
esac
