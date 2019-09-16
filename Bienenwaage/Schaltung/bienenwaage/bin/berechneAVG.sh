#!/bin/bash
datum=`date  +%Y%m%d`
avg=`mysql -u bienen -pbienen  -h 192.168.2.25 -D bienen  -e "select avg(value) from gewicht where datum='$datum' "`
val=`echo $avg |cut -f 2 -d " "`
echo $val
mysql -u bienen -pbienen  -h 192.168.2.25 -D bienen  -e "insert into GewichtTag values (null,$val,$datum)"
exit 0
