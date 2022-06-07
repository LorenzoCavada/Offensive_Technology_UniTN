import subprocess
ip = [
  {
    "start": "172.16.16.2",
    "end": "172.16.16.1"
  },
  {
    "start": "2.4.6.10",
    "end": "2.4.6.9"
  },  
  {
    "start": "3.5.7.17",
    "end": "3.5.7.18"
  },  
  {
    "start": "10.0.0.2",
    "end": "10.0.0.1"
  },  
]
f = open("interface.txt", "r")
count = 0 #to know which set of ip I need to use
for i, line in enumerate(f):  
  if line != "\n": #avoid empty lines
    line = line.split(" ")
    start = [line[0], line[1]]
    end = [line[-2], line[-1].strip("\n")]

    ssh_id = start[0] #setting IP to the first machine of the line
    interface = "sudo ifconfig " + start[1] + " " + ip[count]["start"] + " netmask 255.255.255.248"
    subprocess.call(['bash','bash_executecommands.sh', ssh_id, interface])
    print("interface for " + ssh_id + ": " + start[1] + " setted with IP: " + ip[count]["start"])

    ssh_id = end[0] #setting IP to the second machine of the line
    interface = "sudo ifconfig " + end[1] + " " + ip[count]["end"] + " netmask 255.255.255.248"
    subprocess.call(['bash','bash_executecommands.sh', ssh_id, interface])
    print("interface for "+ ssh_id + ": " + end[1] + " setted with IP: " + ip[count]["end"])

    count = count + 1