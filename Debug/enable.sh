#!/bin/bash
set -e

iptables -Z ; iptables -F ; iptables -t nat -F; iptables -t mangle -F

iptables -A OUTPUT -p tcp -s 192.168.1.96 -d 192.168.1.96 -j NFQUEUE --queue-num 0 

#iptables -t nat -A POSTROUTING -j MASQUERADE
#iptables -A OUTPUT -p icmp -j DROP
#iptables -A FORWARD -p tcp -j NFQUEUE --queue-num 0


