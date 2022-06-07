# Lorenzo Cavada Mat. 220502
First exercise of OffTech about routing.<br><br>
  

## Setup of the experiment.
For setting up the experiment follow the instruction [here](https://www.isi.deterlab.net/file.php?file=/share/shared/Internetworking).  
After that the fresh experiment is created and swapped-in `SSH` to your machine and clone this [repo](https://github.com/LorenzoCavada/internetworking-cvd) in the main folder.

## Set the Interface

The first step needed to complete the exercise is to set the interface of each node of the experiment. Basically what is needed is to understand which interface is active on each machine and apply an appropiate IP address.  
For this exercise, I used the same IP proposed by David Morgan.  
  
To understand which interface is on start the `init.sh` script:  
  
    bash init.sh  
      
This will check the active interface and will generate a file called `interface.txt` with the result of the scan.  
Now is possible to set the IP, to do so simply start the `setinterface.py` script.  
  
    python setinterface.py  
      
In this step should be prompt by the script which IP has been assigned to which interface of which node. The script is very basic, it scans the previously created txt file
and parses it to extrapolate the active interface. Then, for each interface, a specific IP is set.  
The decision of the IP is previously made and is fixed, the structure of the txt file is always the same making possible to do some default assumption while parsing it. 
  
To actually set the IP an SSH connection is made indeed the script, after reading the device name and the active interface, call the `bash_executecommands.sh` script
which is a general script also use later on that accept two parameters: the name of the node on which the command/s should be performed and the list of commands to perform, in the following form:  
    
    commands1; commands2; ...
  
After that, the script is ended is now possible to `SSH` to a machine and try to ping with the directly connected machine.
For example, by connection with SSH to <em>NWworkstation1</em> should be possible to ping `172.16.16.1` (<em>NWrouter</em>) but the traffic directly to the other machine should not be able to reach the destination.

## Set the Routing  
  
The next step is to set the routing table in order to allow each machine to contact the other. To do so, from the main machine, start the `setrouting.py` script: 
  
    python setrouting.py  
      
This script will simply create an SSH connection with each node and will perform a set of pre-defined commands that will enable the routing.  
In this phase are also implemented the basics firewall rule in the <em>ISrouter</em> to prevent direct access to the private IP.  
  
By connection again with the NWworkstation1 should now be possible to ping also the `2.4.6.9` (upper interface of the <em>ISrouter</em>) while trying to directly 
ping `10.0.0.2` (<em>SWworkstation1</em>) will not provide any response due to the firewall rules implemented in the <em>ISrouter</em>.  
  
## Set NAT and Port Forwarding  
  
Each request is for now traveling on the network with the original source address, so, for example, the ping coming from <em>NWworkstation1</em> shows as source IP `172.16.16.2`.  
This has two major consequences, first of all, it reveals some information about the structure of the internal network, and, second, it does not allow the packets to get through the <em>ISrouter</em>. To resolve that is possible to implement some NAT rules that will statically change the internal private IP with the external public
IP of the two border router (the <em>SWrouter</em> and the <em>NWrouter</em>).  
  
On the <em>SWworkstation1</em> is also running an apache web server that needs to be reachable from the outside. To do so is possible to apply a port forwarding
rule that will redirect all the traffic with specific characteristics, in this case, TCP traffic which is directed to the `port 80` of the <em>SWrouter</em>.  
  
To implement both the NAT and the Port Forwarding rules run, always from the main machine, the `setportnat.py` script.  
  
    python setportnat.py
  
This script will once again check the `interface.txt` file to understand with which interface the <em>NWrouter</em> and the <em>SWrouter</em> are facing
the external network and will proceed by connecting with `SSH` to them and apply the rules.  
  
Now should be possible from the <em>NWworkstation1</em> to ping the external interface of the <em>SWrouter</em> and also should be possible to connect to the
apache web server by typing:  
  
    lynx 3.5.7.18

(May be necessary to install `yarn`, to do so type `sudo apt install yarn`).  
  
Is also interesting to see how the `NAT` rules are working. To do so connect throw `SSH` to the <em>ISrouter</em> and start sniffing on any of the two active interface.  
  
    sudo tcpdump -nnti ethX
  
Now generate some traffic by trying to connect the webserver on <em>SWworkstation1</em> (`lynx 3.5.7.18`) and you should see in the sniffed packets how the source IP is not anymore `172.16.16.2` but has been changed with the one of the <em>NWrouter</em> (`2.4.6.10`) thanks to the `NAT` rule previously implemented.
