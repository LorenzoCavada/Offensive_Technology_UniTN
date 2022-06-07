#!/bin/bash

echo "Configuring the server installing the webserver"

ssh server.dos-cvd.offtech "/share/education/TCPSYNFlood_USC_ISI/install-server" &> /dev/null

echo "Removing from the server the TCP Cookie"
ssh server.dos-cvd.offtech "sudo sysctl -w net.ipv4.tcp_syncookies=0; sudo sysctl -w net.ipv4.tcp_max_syn_backlog=10000;" &> /dev/null

echo "Preparing the attacker by installing flooder"

ssh attacker.dos-cvd.offtech "/share/education/TCPSYNFlood_USC_ISI/install-flooder" &> /dev/null

echo "Donfiguration done."
