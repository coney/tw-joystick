#!/bin/bash

MODULE=joystick
MODULE_FILE=${MODULE}.ko

function load_driver {
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
