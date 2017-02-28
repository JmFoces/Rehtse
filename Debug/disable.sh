iptables -Z ; iptables -F ; iptables -t nat -F ;iptables -t mangle -F
iptables -t nat -A POSTROUTING -j MASQUERADE
iptables -A OUTPUT -p icmp -j DROP
