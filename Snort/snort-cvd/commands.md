### Commands    
    
    # Start snord using the snort.conf configuration file
    sudo snort --daq nfq -Q -c snort.config -l alerts 

    # Start snort as before but force snort to listen on the interface connected with the router
    sudo snort --daq nfq -Q -c snort.config -l alerts --daq-var device=eth1

    # Install the flooder
    /share/education/TCPSYNFlood_USC_ISI/install-flooder

    # Start the flooding
    sudo timeout 100 flooder --dst 100.1.10.10 --highrate 100000 --srcmask 255.255.255.255 --src 100.1.5.10

    # Create a tpdump
    sudo tcpdump -i eth -s 0 -w name.pcap

    # Get number of packets/s from a pcap file
    sudo /share/education/SecuringLegacySystems_JHU/process.pl name.pcap

    # Require a file from the server
    java -jar FileClient.jar candice monday server ducky.txt

    # Manually send UDP traffic to the server
    sudo echo -n "hi" >/dev/udp/100.1.10.10/7777

    # Force the internal machine to send the traffic to snort
    sudo route add -host server gw snort

    # Require all the .txt file
    java -jar FileClient.jar bob password server export.txt & \
    java -jar FileClient.jar joe password1 server classified.txt & \
    java -jar FileClient.jar candice monday server ducky.txt & \
    java -jar FileClient.jar billy thursday server users.txt

    # Require the .xml file
    java -jar FileClient.jar bob password server export.xml

    # Snort Preprocessor
    sudo cp -rf /usr/local/snort-2.9.2.2/src/dynamic-preprocessors/include/ ..