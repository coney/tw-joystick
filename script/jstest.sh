#!/bin/bash

PROCFILE=/proc/twjoystick/test
PLAYERS=2
BUTTONS=8
INTERVAL=0.2

for ((p=0;p<$PLAYERS;p++)); do
    for((i=1;i<=$BUTTONS;i++)); do
        echo press button $i of player $p
        echo -n $p $i > $PROCFILE
        sleep $INTERVAL
        
        echo release button $i of player $p
        echo -n $p -$i > $PROCFILE
        sleep $INTERVAL
    done
done
