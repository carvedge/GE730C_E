#!/bin/sh

while [ 1 ] ; do
 read cnt < /tmp/DataDisk/app/ram/2.txt
 	if echo "$cnt" | grep -q "dev/null"
 	then
		echo "dev/null"
	else
		echo "exec cmd"
		eval $cnt
		echo dev/null > /tmp/DataDisk/app/ram/2.txt
	fi
 sleep 1
done
