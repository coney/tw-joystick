#!/bin/bash -e

SVC_NAME=twjoyd
BIN_PATH=./${SVC_NAME}
SVC_PATH=./script/${SVC_NAME}.sh
TARGET_BINDIR=/usr/local/bin

function log {
    echo $*
}

function install_bin {
    if ! [[ -x $BIN_PATH ]]; then
        log "can't find bin file '$BIN_PATH'"
        exit -1
    fi
    
    install -v $BIN_PATH $TARGET_BINDIR/
}

function install_service {
    if ! [[ -x $SVC_PATH ]]; then
        log "can't find service script '$SVC_PATH'"
        exit -1
    fi
    
    install -v $SVC_PATH /etc/init.d/$SVC_NAME
    update-rc.d $SVC_NAME defaults
}


install_bin
install_service



