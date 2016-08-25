#!/bin/bash
set -e

iptables -Z ; iptables -F ; iptables -t nat -F; iptables -t mangle -F

iptables -A OUTPUT -p tcp -s 192.168.1.96 -d 192.168.1.96 -j NFQUEUE --queue-num 0 


