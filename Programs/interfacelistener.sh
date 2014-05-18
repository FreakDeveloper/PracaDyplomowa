#!/bin/bash
export LC_NUMERIC="en_US.UTF-8";

iter=0;
printf "probe;Download KB/s;Download MB/s;Upload KB/s;Upload MB/s\n" > $1

while :
do
iter=$(($iter+1));
# Press ctrl-c to exit

x=`ifstat -i eth0 $2 1 | tail -1 | tr '\t' ' '`;  #tail -1 takes the DL and UL speeds in KB/S
x1=`echo $x | cut -d ' ' -f1`; 
x2=`echo $x | cut -d ' ' -f2`;

x3=$(echo "scale=3; $x1/1024" | bc ); # Convert DL rate to MB/S
x4=$(echo "scale=3; $x2/1024" | bc );
printf "%d;%0.3f;%0.3f;%0.3f;%0.3f\n" "$iter" "$x1" "$x3" "$x2" "$x4" >> $1

done