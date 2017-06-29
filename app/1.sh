#!/bin/sh


CheckProcess(){
  if [ "$1" = "" ];
  then
  return 1
  fi
  PROCESS_NUM=`ps  | grep "$1" | grep -v "grep" | wc -l`
  if [ $PROCESS_NUM -gt 0 ]
  then
  return 0
  else
  return 1
  fi
}

while [ 1 ] ; do
 read cnt < /tmp/DataDisk/app/ram/1.txt
 	if echo "$cnt" | grep -q "killall 1.mod"
 	then
		echo "CheckProcess 0"
	    CheckProcess "1.mod"
		RET=$?
		if [ $RET -eq 0 ]
		then
		eval $cnt
		echo $cnt
		fi
		echo 
		echo dev/null > /tmp/DataDisk/app/ram/1.txt
	elif echo "$cnt" | grep -q "1.mod"
	then
		echo "CheckProcess 1"
		CheckProcess "1.mod"
		RET=$?
		if [ $RET -eq 1 ]
		then
		eval $cnt
		echo $cnt
		fi
		echo dev/null >  /tmp/DataDisk/app/ram/1.txt
	else
		echo "None of the condition met"
	fi
 echo "sleep"
 sleep 1
done
