#!/bin/sh

#example: /opt/app/vsn.sh http://ivs.carvedge.com/vsn/1.2.tar.gz
if [ $# -lt 1 ]; then
	echo "need parameter"
	exit
fi

#detect-weather-self-is-runing?
cnt=`ps  | grep "/tmp/DataDisk/app/vsn.sh http://" | grep -v grep | wc -l`
if [ $cnt -gt 2 ]; then
	exit
fi

#delete-old-file
rm -rf /tmp/DataDisk/app/ram/vsn/*

#get-file-from-svr
#wget -c $1  -O /tmp/DataDisk/app/ram/vsn/0.tar.gz
/tmp/DataDisk/app/curl -o /tmp/DataDisk/app/ram/vsn/0.tar.gz $1
#unzip
tar -zxvf /tmp/DataDisk/app/ram/vsn/0.tar.gz -C /tmp/DataDisk/app/ram/vsn/

#run-shell
chmod 777 /tmp/DataDisk/app/ram/vsn/vsn.sh
/tmp/DataDisk/app/ram/vsn/vsn.sh

echo "done"

exit
