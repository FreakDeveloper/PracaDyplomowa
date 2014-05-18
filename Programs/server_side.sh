#!/bin/bash

files_folder="../FILES/";
file20MB="../FILES/file20mb.out";
file100MB="../FILES/file100mb.out";
file500MB="../FILES/file500mb.out";
iterations=40;

output_folder="../OUT/";
sendoutput20MB="../OUT/sendoutput20MB.out";
sendoutput100MB="../OUT/sendoutput100MB.out";
sendoutput500MB="../OUT/sendoutput500MB.out";

PORT_NUM=5000;
IP_NUM="192.168.1.1";

#create FILES folder for testng
if [ ! -d $files_folder ]
then
    mkdir $files_folder;
fi

#create FILES folder for testng
if [ ! -d $output_folder ]
then
    mkdir $output_folder;
fi

if [ ! -f $file20MB ]
then
    dd if=/dev/urandom of=../FILES/file20mb.out bs=20M count=1;
fi

if [ ! -f $file100MB ]
then
    dd if=/dev/urandom of=../FILES/file100mb.out bs=100M count=1;
fi

if [ ! -f $file500MB ]
then
    dd if=/dev/urandom of=../FILES/file500mb.out bs=500M count=1;
fi


echo "Test 20MB " > $sendoutput20MB
for (( i=0; $i < $iterations; i++ )) ; do
		port=$(($PORT_NUM+$i));
		echo "Sending to:" $port $IP_NUM $file20MB;
       ./server "$port" "$IP_NUM" "$file20MB" &>> "$sendoutput20MB"
       sleep 2
done

echo "Test 100MB " > $sendoutput100MB
for (( i=0; $i <= $iterations; i++ )) ; do
		port=$(($PORT_NUM+$i));
		echo "Sending to:" $port $IP_NUM $file100MB;
       ./server "$port" "$IP_NUM" "$file100MB" &>> "$sendoutput100MB"
       sleep 2
done

echo "Test 500MB " > $sendoutput500MB
for (( i=0; $i <= $iterations; i++ )) ; do
		port=$(($PORT_NUM+$i));
		echo "Sending to:" $port $IP_NUM $file500MB;
       ./server "$port" "$IP_NUM" "$file500MB" &>> "$sendoutput500MB"
       sleep 2
done

#if [ -d $files_folder ]
#then
#    rm -rf $files_folder;
#fi