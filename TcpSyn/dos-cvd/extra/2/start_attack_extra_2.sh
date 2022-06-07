#!/bin/bash

echo -e "Starting the attack."

eth_client=$(ssh client.dos-cvd.offtech "sudo ip route show to match 1.1.1.1" | tr -d '\n' |tr ' ' '\n' | grep eth*)

eth_client=$(echo $eth_client | awk '{print $NF}')

echo "Start the TCPDUMP on the client saving the result into a file called filter.pcap" &
ssh client.dos-cvd.offtech "sudo timeout 180s tcpdump -nnti $eth_client -w filter.pcap tcp" &> /dev/null &

echo "Start the legitimate traffic from the client" &
ssh client.dos-cvd.offtech "cd ./dos-cvd/script/; sudo timeout 180s bash traffic-gen.sh 1.1.1.1" &> /dev/null &

sleep 30

echo "Start the SYN Flooding from the attacker"& 
ssh attacker.dos-cvd.offtech "cd ./dos-cvd/script/; sudo timeout 120s bash syn-dos.sh 2.2.2.1 300 2.2.2.2 255.255.255.0" &> /dev/null &

sleep 150
echo "FINISH"