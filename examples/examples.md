# Change DNS
## Just change the result of the address returned by the server by 8.8.8.8.
```javascript
{"debuglevel":0,
"patterns":[
    {
        "match":{
            "bpf":"udp dst port 53", --- Match udp packets with source port 53. 
            "regex":".{2}\\x81\\x80.*google\\x03com.*" --- Filter packets by regex, for example all of them having an encoded dns response A for google.com
        },
        "replacement":{
            "regex":"(.{40})(.{4})", --- Drop the replacement here. Group 1 is the offset that wont be modified. Group 2 are 4 bytes to be replaced by \x08.
            "replacement":"$1\\x08\\x08\\x08\\x08"
        }
    }
]}
```

```bash
./run.sh change_dns.json
```

### This is the output from Rehtse on trace level:
```
read 152 bytes from netfilter queue :
...
PACKET DATA 45000048147a000036119d6b08080808c0a802080035d86c0034d9177a498180000100010000000006676f6f676c6503636f6d0000010001c00c000100010000004c0004`d83ad3ee`

PACKET MOD 45000048147a000036119d6b08080808c0a802080035d86c003475317a498180000100010000000006676f6f676c6503636f6d0000010001c00c000100010000004c00040`8080808`
...
```

### Results 
Without Rehtse running
nslookup google.com
Server:     8.8.8.8
Address:    8.8.8.8#53

Non-authoritative answer:
Name:   google.com
Address: 216.58.211.238

With Rehtse running
nslookup google.com
Server:     8.8.8.8
Address:    8.8.8.8#53

Non-authoritative answer:
Name:   google.com
Address: 8.8.8.8



# Change HTTP path on an HTTP-GET
## Just change /debian/ by /ubuntu-xenial/
```javascript
{   
    "debuglevel":0,
    "patterns":[
        {
            "match":{
                "bpf":"tcp dst port 80",
                "regex":"GET /debian/ HTTP/1.1.*"
            },
            "replacement":{
                "regex":"(GET )(/debian/)( HTTP/1.1)",
                "replacement":"$1/ubuntu-xenial/$3"
            }
        }
    ]
}
```

```bash
./run.sh change_http_get_uri.json
```

### This is the output from Rehtse on trace level:
```
Got key_S c0a8020889e2222e0678c95000
Got key_D 89e2222ec0a8020806500078c9
Flow Tracker access with key c0a8020889e2222e0678c95000
Flow Tracked already 
TCPFlow  handling packet 
TCPFlow c0a8020889e2222e0678c95000
TCPFlow Packet is PSH
Scanner checking packet 
PatternBPF: tcp dst port 80 REGEX: GET /debian/ HTTP/1.1.*checking packet...
PACKET DATA 450000816cc0400040065ef6c0a8020889e2222ec97800503ba52d279da55102501800e528570000474554202f64656269616e2f20485454502f312e310d0a486f73743a20667470322e64652e64656269616e2e6f72670d0a557365722d4167656e743a206375726c2f372e35322e310d0a4163636570743a202a2f2a0d0a0d0a
BPF Program exit code: 65535
BPF Match tcp dst port 80
Packet Matches regex
PatternBPF: tcp dst port 80 REGEX: GET /debian/ HTTP/1.1.*Matched!
PatternBPF: tcp dst port 80 REGEX: GET /debian/ HTTP/1.1.*Updating packet contents...
BASE PAYLOAD 474554202f64656269616e2f20485454502f312e310d0a486f73743a20667470322e64652e64656269616e2e6f72670d0a557365722d4167656e743a206375726c2f372e35322e310d0a4163636570743a202a2f2a0d0a0d0a
Payload mod PAYLOAD 474554202f7562756e74752d78656e69616c2f20485454502f312e310d0a486f73743a20667470322e64652e64656269616e2e6f72670d0a557365722d4167656e743a206375726c2f372e35322e310d0a4163636570743a202a2f2a0d0a0d0a
PatternBPF: tcp dst port 80 REGEX: GET /debian/ HTTP/1.1.*Updated packet content
Keeping consistency of TCP Flows 51576->80
Current SEQ 1000680743
MOD SEQ 1000680743
Current ACK 2644857090
MOD ACK 2644857090
TCPFlow Updated Packet Content 
```

### From this moment Rehtse will take care of re-adjusting SEQ and ACK numbers on each packet belonging to this connection until it closes:
```Keeping consistency of TCP Flows 80->51576
Current SEQ 2644857090
MOD SEQ 2644857090
Current ACK 1000680839
MOD ACK 1000680832
TCPFlow Updated Packet Content 
```

### Results
Without Rehtse running
```curl -s -v http://ftp2.de.debian.org/debian/ > /dev/null 
GET /debian/ HTTP/1.1
Host: ftp2.de.debian.org
User-Agent: curl/7.52.1
Accept: */*

HTTP/1.1 200 OK
```
With Rehtse running
```curl http://ftp2.de.debian.org/debian/ 
<html>...
<p>The requested URL /ubuntu-xenial/ was not found on this server.</p>
...</html>```
