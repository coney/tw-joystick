#!/bin/bash

MODULE=joystick
MODULE_FILE=${MODULE}.ko

function load_driver {
    insmod $MODULE_FILE
}

function unload_driver {
    rmmod $MODULE
}




case $1 in
    load)
        echo "loading $MODULE"
        load_driver;;

    unload)
        echo "unloading $MODULE"
        unload_driver;;
esac
