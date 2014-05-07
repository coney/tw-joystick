tw-joystick
===========

joystick driver for game arcade in TW Xi'an office


Dependencies
-----------

##driver
  you need the kernel header corresponding to you kernel version.
  if you are going to cross-compile it, you need the kernel header which matches your device's kernel version. of course, the cross toolchain is required. for raspberry PI you can check out this repo: https://github.com/coney/raspi 

##joystick daemon & libsmsg
1. [libpoco](http://pocoproject.org/) for buiding the daemon application
2. [libserial](http://libserial.sourceforge.net/) for communication through serial

##arduino-io
1. [ino](http://inotool.org/) for building and debugging the arduino source
2. [picocom](https://code.google.com/p/picocom/) for communication with arduino through serial
3. [arduino](http://arduino.cc/en/Main/Software) provides the libraries for compiling the arduino source
