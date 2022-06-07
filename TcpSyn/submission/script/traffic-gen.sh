#!/bin/bash

echo "Sending request to $1"
while :
do
curl $1 &> /dev/null &
echo "request send"
sleep 1
done