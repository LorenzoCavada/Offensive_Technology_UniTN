TCP DUMP

sudo tcpdump -w filter.pcap -nnti ethX tcp

EXTRA 1

Try by blocking tcp syn ack coming from the server and directed to the attacker.

Perform syn flooding with fixed attacker IP source
sudo bash syn-dos.sh 5.6.7.8 200000 1.1.2.4 255.255.255.255

Without firewall, the attacker is sending malicious SYN ACK but is also receiving unexpecting SYN+ACK sending a RST which free the memory allocated by the server.

This should stop all the packet with SYN ACK flag setted to 1
	iptables -I INPUT 1 -s <source> -p tcp --tcp-flags SYN,ACK SYN,ACK -j DROP
To revert
	iptables -D INPUT -s <source> -p tcp --tcp-flags SYN,ACK SYN,ACK -j DROP
	
This should stop all the packet send with RST flag setted to 1 -> THIS WORK AS EXPECTED, APPLY IT TO THE ATTACKER
	iptables -I OUTPUT 1 -d 5.6.7.8 -p tcp --tcp-flags RST RST -j DROP -> TOOOPPPP
To revert
	iptables -D OUTPUT -d 5.6.7.8 -p tcp --tcp-flags RST RST -j DROP
