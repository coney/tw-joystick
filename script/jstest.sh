#!/bin/bash

PROCFILE=/proc/twjoystick/test
PLAYERS=2
# 4 directions + ABCXYZ
BUTTONS=10
INTERVAL=0.1

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
