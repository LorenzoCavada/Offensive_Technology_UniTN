import subprocess
routing = [
  {
    "name": "NWworkstation1",
    "commands": ["route add -net 2.4.6.8 netmask 255.255.255.248 gw 172.16.16.1",
                "route add -net 3.5.7.16 netmask 255.255.255.248 gw 172.16.16.1",
                "route add -net 10.0.0.0 netmask 255.255.255.248 gw 172.16.16.1"]
  },
  {
    "name": "SWworkstation1",
    "commands": ["route add -net 3.5.7.16 netmask 255.255.255.248 gw 10.0.0.1",
                "route add -net 2.4.6.8 netmask 255.255.255.248 gw 10.0.0.1",
                "route add -net 172.16.16.0 netmask 255.255.255.248 gw 10.0.0.1"]
  },
  {
    "name": "NWrouter",
    "commands": ["route add -net 3.5.7.16 netmask 255.255.255.248 gw 2.4.6.9",
                "route add -net 10.0.0.0 netmask 255.255.255.248 gw 2.4.6.9"]
  },
  {
    "name": "SWrouter",
    "commands": ["route add -net 2.4.6.8 netmask 255.255.255.248 gw 3.5.7.17",
                "route add -net 172.16.16.0 netmask 255.255.255.248 gw 3.5.7.17"]
  },
  {
    "name": "ISrouter",
    "commands": ["route add -net 172.16.16.0 netmask 255.255.255.248 gw 2.4.6.10",
                "route add -net 10.0.0.0 netmask 255.255.255.248 gw 3.5.7.18",
                "iptables -I FORWARD -d 192.168.0.0/16 -j DROP",
                "iptables -I FORWARD -d 172.16.0.0/12 -j DROP",
                "iptables -I FORWARD -d 10.0.0.0/8 -j DROP",
                "iptables -I FORWARD -s 192.168.0.0/16 -j DROP",
                "iptables -I FORWARD -s 172.16.0.0/12 -j DROP",
                "iptables -I FORWARD -s 10.0.0.0/8 -j DROP"]
  }
]

for node in routing:
  ssh_commands = ""
  for command in node["commands"]:
    ssh_commands += "sudo " + command + "; "
  print("Setting routing to " + node["name"])
  subprocess.call(['bash','bash_executecommands.sh', node["name"], ssh_commands])

print("SCRIPT ENDED")

