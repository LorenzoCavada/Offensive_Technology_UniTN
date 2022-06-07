import subprocess
devices = [
  {
    "name": "NWrouter",
    "commands": ["iptables -t nat -A POSTROUTING -o ? -s 172.16.16.0/29 -j SNAT --to 2.4.6.10"]
  },
  {
    "name": "SWrouter",
    "commands": ["iptables -t nat -A POSTROUTING -o ? -s 10.0.0.0/29 -j SNAT --to 3.5.7.18",
                "iptables -t nat -A PREROUTING -i ? -d 3.5.7.18/32 -p tcp --dport 80 -j DNAT --to 10.0.0.2"]
  }
]

f = open("interface.txt", "r")
for i, line in enumerate(f):  
  if line != "\n":
    line = line.split(" ")
    if line[0] == "NWrouter":
      device = devices[0]
      ssh_commands = ""
      for command in device["commands"]:
        ssh_commands += "sudo " + command.replace("?", line[1]) + "; "
      print("Adding NAT roule to " + device["name"] + " at interface: " + line[1])
      subprocess.call(["bash", "bash_executecommands.sh", device["name"], ssh_commands])   
    if line[-2] == "SWrouter" and line[0] == "ISrouter":
      device = devices[1]
      ssh_commands = ""
      for command in device["commands"]:
        ssh_commands += "sudo " + command.replace("?", line[-1].strip("\n")) + "; "
      print("Adding NAT and Port Forwarding rule to " + device["name"] + " at interface: " + line[-1].strip("\n"))
      subprocess.call(["bash", "bash_executecommands.sh", device["name"], ssh_commands]) 

print("SCRIPT ENDED")

