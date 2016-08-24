#!/bin/bash
set -e

iptables -Z ; iptables -F ; iptables -t nat -F; iptables -t mangle -F

iptables -A INPUT -j NFQUEUE --queue-num 0
iptables -A OUTPUT -j NFQUEUE --queue-num 0 


