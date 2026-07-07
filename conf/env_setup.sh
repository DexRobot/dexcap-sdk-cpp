#!/bin/bash

OLDIFS="$IFS"
IFS=$'\n'

user=`whoami`

sudo apt install -y libssl-dev libyaml-cpp-dev libserial-dev libusb-1.0-0
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
