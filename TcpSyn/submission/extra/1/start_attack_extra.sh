#!/bin/bash

echo -e "Starting the attack."

eth_client=$(ssh client.dos-cvd.offtech "sudo ip route show to match 5.6.7.8" | tr -d '\n' | awk '{print $NF}')

echo "Start the TCPDUMP on the client saving the result into a file called filter.pcap" &
ssh client.dos-cvd.offtech "sudo timeout 180s tcpdump -nnti $eth_client -w filter.pcap tcp" &> /dev/null &

echo "Start the legitimate traffic from the client" &
ssh client.dos-cvd.offtech "cd ./dos-cvd/script/; sudo timeout 180s bash traffic-gen.sh 5.6.7.8" &> /dev/null &

sleep 30

echo "Start the SYN Flooding from the attacker"& 
ssh attacker.dos-cvd.offtech "cd ./dos-cvd/script/; sudo timeout 120s bash syn-dos.sh 5.6.7.8 300 1.1.2.4 255.255.255.255" &> /dev/null &

sleep 150
echo "FINISH"