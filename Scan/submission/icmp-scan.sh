#!/bin/bash

SUB='Unreachable'

echo "Starting the scan of 5.6.7.0/24 using the ping command"
for i in {1..254}
do (
  host=$(ping -c 1 5.6.7.$i)

  if [[ "$host" != *"$SUB"* ]]; then
    echo "Host 5.6.7.$i is up"
  fi
  )
done

