#!/bin/bash

sudo nmap -sP 5.6.7.0/24

sudo nmap -sA 5.6.7.0/24

sudo nmap -p- -sS 5.6.7.8 #scan all ports on a specific machine using half open scanning

sudo nmap -p 1-1500 -sS 5.6.7.8 #scan the first 1500 ports on a specific machine using half open scanning

sudo nmap -p 1-1500 -sX 5.6.7.8 #scan the first 1500 ports on a specific machine using XMAS scanning

sudo nmap -p 1-1500 -sA 5.6.7.8 #scan the first 1500 ports on a specific machine using TCP ACK scanning

sudo nmap -O 5.6.7.8 #get the OS of the machine

sudo nmap -sV 5.6.7.8 #get the Service Version of the machine


