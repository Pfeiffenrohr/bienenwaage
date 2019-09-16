#!/bin/bash
ps aux |grep bienenwaage.jar |grep -v grep >/dev/null
if [ $? -ne 0 ]; then
echo "Starte bienenwaage..."
/home/pi/bienenwaage/bin/startBienenwaage.sh & >> /home/pi/bienenwaage/log/bienenwaage.log
fi
find /share/tmp/bienenwaage.jar -cnewer  /home/pi//bienenwaage/bin/bienenwaage.jar >/tmp/findbienenwaage.tmp
grep bienenwaage /tmp/findbienenwaage.tmp
if [ $? -eq 0 ]; then
        pid=`ps aux |grep bienenwaage.jar |grep -v grep | awk -F' ' '{print $2}'
`
        echo "Pid = $pid"
        kill $pid
        echo "$pid gekillt ..."
        sleep 10
        ps aux |grep bienenwaage.jar |grep -v grep >/dev/null
        if [ $? -eq 0 ]; then
                 kill -9 $pid
                 sleep 5
        echo " $pid mit -9 gekillt ..."
        fi
         ps aux |grep bienenwaage.jar |grep -v grep >/dev/null
        if [ $? -eq 0 ]; then
                echo "Konnte Prozess nicht stoppen .."
                exit 1
        fi
        cp /share/tmp/bienenwaage.jar /home/pi/bienenwaage/bin/bienenwaage.jar
        sleep 20
	/home/pi/bienenwaage/bin/startBienenwaage.sh & >> /home/pi/bienenwaage/log/bienenwaage.log
fi

