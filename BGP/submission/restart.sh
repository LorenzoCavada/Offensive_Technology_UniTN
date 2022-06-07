#!/bin/bash

ssh client.bgp-cvd.offtech "sudo /etc/init.d/quagga restart"
ssh server.bgp-cvd.offtech "sudo /etc/init.d/quagga restart"
ssh attacker.bgp-cvd.offtech "sudo /etc/init.d/quagga restart"
ssh asn1.bgp-cvd.offtech "sudo /etc/init.d/quagga restart"
ssh asn2.bgp-cvd.offtech "sudo /etc/init.d/quagga restart"
ssh asn3.bgp-cvd.offtech "sudo /etc/init.d/quagga restart"
ssh asn4.bgp-cvd.offtech "sudo /etc/init.d/quagga restart"
