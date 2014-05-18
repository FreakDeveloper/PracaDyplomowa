#!/bin/bash

files_folder="../IN/";
file20MB="../IN/file20mb.out";
file100MB="../IN/file100mb.out";
file500MB="../IN/file500mb.out";

output_folder="../OUTCL/";
sendoutput20MB="../OUTCL/sendoutput20MB.out";
sendoutput100MB="../OUTCL/sendoutput100MB.out";
sendoutput500MB="../OUTCL/sendoutput500MB.out";

PORT_NUM=5000;
IP_NUM="192.168.1.1";
iterations=40;

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

echo "Test 20MB " > $sendoutput20MB 
for (( i=0; $i < $iterations; i++ )) ; do
		port=$(($PORT_NUM+$i));
		echo "Receiving from:" $port $IP_NUM $file20MB;
       ./client "$IP_NUM" "$port" "$file20MB" &>> "$sendoutput20MB"
       sleep 3
done
sleep 1

echo "Test 100MB " > $sendoutput100MB 
for (( i=0; $i < $iterations; i++ )) ; do
		port=$(($PORT_NUM+$i));
		echo "Receiving from:" $port $IP_NUM $file100MB;
       ./client "$IP_NUM" "$port" "$file100MB" &>> "$sendoutput100MB"
       sleep 3
done
sleep 1

echo "Test 500MB " > $sendoutput500MB 
for (( i=0; $i < $iterations; i++ )) ; do
		port=$(($PORT_NUM+$i));
		echo "Receiving from:" $port $IP_NUM $file500MB;
       ./client "$IP_NUM" "$port" "$file500MB" &>> "$sendoutput500MB"
       sleep 3
done
sleep 1