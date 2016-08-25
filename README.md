# Rehtse
br_netfilter+Iptables+NFQueue+BPF+Regex ---> Easy way to modify network packets on the fly!

  Thanks to:
  
    https://github.com/pellegre/libcrafter
    https://github.com/ytakano/radix_tree
    
  Both of them maid things to work here.

You will need:

    apt-get install libboost-all-dev libnetfilter-queue1 libnetfilter-queue-dev libpcap0.8 libpcap0.8-dev linux-headers-amd64 gcc g++ make 
    Follow instructions to install libcrafter. 
  
To use it:

    1 cd Debug; make clean; make all;
    2 edit your config.json file. Take a look at Debug/config.json for an example.
    3 write your iptables rule to dispatch packets to Rehtse -> iptables -A FORWARD -j NFQUEUE --queue-num 0.
    4 just ./RehtSe
    
The project works now on Eclipse Neon. 
  
