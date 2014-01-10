#!/bin/bash

for((i=1;i<=10;i++)); do
    echo press button $i
    echo -n $i > /proc/twjs
    sleep 1
    
    echo release button $i
    echo -n -$i > /proc/twjs
    sleep 1
done