#!/bin/sh


if [ $# -lt 2 ]; then
	echo "need parameter"
	exit
fi

### 加锁避免同时重复运行
LOCK="/tmp/DataDisk/app/ram/uploadimage.lock"
if ! ( set -o noclobber; echo "$$" > "$LOCK") 2> /dev/null;
then
    echo "Cannot run multiple."
    exit 1
fi
trap 'rm -f "$LOCK"; exit $?' INT TERM EXIT

PWD="/tmp/DataDisk/app/image/"
FILES=` find $PWD -name *.jpg `

### 遍历文件
for file in $FILES
do
   cmd0="/tmp/DataDisk/app/curl -F 'dat=@"
   cmd1=$file
   name=`echo $file | sed 's/.*\/\(.*\)\..*/\1/'`
   cmd2="' '"
   cmd3="http://""$1"":1936/svr/box.php?act=g&bid=""$2""&pre=0&nme=""$name"
   cmd4="'"
   cmd="$cmd0""$cmd1""$cmd2""$cmd3""$cmd4"
   rep=`eval $cmd`
   if [ $rep == 0 ]
   then
		echo "upload success"
		rm $file
   else
		echo "upload failed"
   fi
   echo $cmd
   sleep 3
   echo $file
done
### 解除锁定
rm -f $LOCK
trap - INT TERM EXIT
