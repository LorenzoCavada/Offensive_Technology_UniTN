from scapy.all import *
import matplotlib.pyplot as plt
import sys

"""
Structure of an element in pkt_list
{
  c_id: src-port of the client,
  pkts: [pkt],
  completed: True/False,
  time: XXXms
}
"""

# Function used to check if a packet is already in pkt_list
def find(lst, key, value):
  for i, dic in enumerate(lst):
      if dic[key] == value:
          return i
  return -1


def main():
  # Axis of the graph, y is connection duration, x is connection start time
  x = []
  y = []
  
  pkt_list = []

  try:
    pcap = rdpcap(sys.argv[1])
  except:
    print("\nYou need to provide a .pcap file (i.e. python3 wirefish.py filter.pcap)\n")
    exit()
  
  # Here for eack packets is inserted in the pkt_list based on the source port of the client
  # If is not already present in the list a new element is added
  for pkt in pcap:
    port = max(pkt[IP].sport, pkt[IP].dport)
    index = find(pkt_list, "c_id", port)
    if index >= 0:
      pkt_list[index]["pkts"].append(pkt)
    else:
      pkt_list.append({"c_id":port, "pkts":[pkt]})

  # Here each connection is checked and the connection time is calculated
  # For uncompleted connection the time is setted to 200
  for e in pkt_list:
    if e["pkts"][-2][TCP].flags == 'FA' and e["pkts"][-1][TCP].flags == 'A' or e["pkts"][-1][TCP].flags == 'R':  
      e["completed"] = True
      e["time"] = e["pkts"][-1].time - e["pkts"][0].time
    else:
      e["completed"] = False
      e["time"] = 200
    
    # The starting time and the connection time is added to the list used to plot the result
    x.append(e["pkts"][0].time - pkt_list[0]["pkts"][0].time)
    y.append(e["time"])


  plt.rcParams["figure.figsize"] = (30,30) #that's for having a full screen windows 
  plt.plot(x, y, '-p', color='gray',
         markersize=2, linewidth=1,
         markerfacecolor='white',
         markeredgecolor='gray',
         markeredgewidth=2)

  plt.axvline(x=30, color='red', linestyle='--')
  plt.axvline(x=150, color='red', linestyle="--")


  
  # naming the x axis
  plt.xlabel('Connection start time [s]')
  # naming the y axis
  plt.ylabel('Connection duration [s]')  
  # giving a title to the graph
  plt.title('DOS Experiment\nFilename: ' + str(sys.argv[1]))  
  # plotting the result
  plt.show()


if __name__ == "__main__":
  main()