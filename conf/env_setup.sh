#!/bin/bash

OLDIFS="$IFS"
IFS=$'\n'

user=`whoami`

sudo apt install -y libyaml-cpp-dev libserial-dev libusb-1.0-0

match_list=`ls /dev/tty* | grep "USB"`

for dev_info in $match_list; do
    echo $dev_info
    sudo chmod 777 $dev_info
done

match_list=`ls /dev/tty* | grep "ACM"`

for dev_info in $match_list; do
    echo $dev_info
    sudo chmod 777 $dev_info
done

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH