#!/bin/sh
ifconfig eth0 down
ifconfig eth0 hw ether 00:0C:18:EF:FF:ED
ifconfig eth0 up 

/tmp/DataDisk/app/0.sh