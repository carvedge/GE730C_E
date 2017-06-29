#!/bin/sh
### 加锁避免同时重复运行
mkdir  /tmp/DataDisk/app
mkdir  /tmp/DataDisk/app/ram
mount -t ramfs -o size=2m ramfs /tmp/DataDisk/app/ram
mkdir  /tmp/DataDisk/app/ram/vsn
LOCK="/tmp/DataDisk/app/ram/0sh.lock"
LOCK1="/tmp/DataDisk/app/ram/1sh.lock"
LOCK2="/tmp/DataDisk/app/ram/2sh.lock"
if ! ( set -o noclobber; echo "$$" > "$LOCK") 2> /dev/null;
then
    echo "Cannot run multiple."
    exit 1
fi
trap 'rm -f "$LOCK"; exit $?' INT TERM EXIT

echo 1 > /tmp/DataDisk/app/zoom.txt



# 返回2： 有多个执行  返回 0： 只有一个执行 返回1 ：没有执行
CheckProcess(){
  if [ "$1" = "" ];
  then
  return 1
  fi
  PROCESS_NUM=`ps  | grep "$1" | grep -v "grep" | wc -l`
  if [ $PROCESS_NUM -gt 1 ]
  then
  return 2
  elif [ $PROCESS_NUM -gt 0 ]
  then 
  return 0
  else
  return 1
  fi
}

export LD_LIBRARY_PATH=/tmp/DataDisk/app/lib:$LD_LIBRARY_PATH

VideoConfig -c 1 -s 0 -t 1 -r 3 -f 10 -g 4 -v 1 -q 32 -b 800000

VideoConfig --BWM 0

AutoFocus -i 1

mount /tmp/sdcard /tmp/DataDisk/app/image

/etc/connect4G.sh up

cd /tmp/DataDisk/app

chmod 777 0.mod
./0.mod&

while [ 1 ] ; do

 CheckProcess "1.sh"
 RET=$?
 if [ $RET -eq 1 ]
 then
 chmod 777 1.sh
 if [ -e $LOCK1 ]; then
 rm $LOCK1
 fi
 exec ./1.sh &
 fi

  CheckProcess "2.sh"
 RET=$?
 if [ $RET -eq 1 ];
 then
 chmod 777 2.sh
 if [ -e $LOCK2 ]; then
 rm $LOCK2
 fi
 chmod 777 2.sh
 exec ./2.sh &
 fi
 
  
 
 CheckProcess "0.mod"
 RET=$?
 if [ $RET -eq 1 ];
 then
 chmod 777 0.mod
 exec ./0.mod &
 elif [ $RET -eq 2 ]
 then 
 killall 0.mod
 fi


 sleep 2
done

### 解除锁定
rm -f $LOCK
trap - INT TERM EXIT
