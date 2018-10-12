# Solution for Assignment 7

## Section 2

#### Question 1
192.168.0.145:36314 (PKT no. 536)

#### Question 2
128.119.245.12:80 (PKT no. 536)

#### Question 3
192.168.0.145:36314 (PKT no. 536)

## Section 3

#### Question 4
Only SYN bit is set. Sequence number is 0xfc0cfd2a. (PKT no. 118)

#### Question 5
Both SYN and ACK bits are set. Sequence number is 0x04563e62. ACK value is determined by number of bytes received(PKT no. 122)

#### Question 6
Sequence number is 0x6cb0350a. (PKT no. 340)

#### Question 7
I my case the TCP segment containing HTTP POST request in sent at last. So here I have noted data for previous 6 segment(including the the one with HTTP POST request, packet filter 'tcp.stream eq 10').

|Packet Number | Time(relative to capture)| SYN Number | Acknowledged by | Time captured|RTT(in ms)|
|---|:--:|:---:|:---:|:---:|:---:|
|340|30.048513366|0x6cb0350a|0x6cb037a9(353)|30.415479891|367|
|341|30.048687924|0x6cb037a9|0x6cb03d01(356)|30.415559493|367|
|342|30.048697280|0x6cb03d01|0x6cb04259(359)|30.415587715|367|
|343|30.048708950|0x6cb04259|0x6cb047b1(360)|30.415597093|367|
|344|30.048711310|0x6cb047b1|0x6cb04d09(361)|30.415604160|367|
|345|30.049887015|0x6cb04d09|0x6cb05261(362)|30.415611465|366|

> EstimatedRTT = 0.875 \* EstimatedRTT + 0.125 \* SampleRTT

340 : EstimatedRTT = 367

341 : EstimatedRTT = 367 \* 0.875 + 367 \* 0.125 = 367

342 : EstimatedRTT = 367 \* 0.875 + 367 \* 0.125 = 367

343 : EstimatedRTT = 367 \* 0.875 + 367 \* 0.125 = 367

344 : EstimatedRTT = 367 \* 0.875 + 367 \* 0.125 = 367

345 : EstimatedRTT = 367 \* 0.875 + 366 \* 0.125 = 366.875

#### Question 8
Length of packets discussed above

|Packet Number|Length of TCP Segment|
|:---:|:---:|
|340|671 + 32|
|341|1368 + 32|
|342|1368 + 32|
|343|1368 + 32|
|344|1368 + 32|
|345|1368 + 32|

#### Question 9

#### Question 10
None in this case. Using `tcp.analysis.retransmission` filter.

#### Question 11
Mostly, while uploading file 1368 bytes of data is acknowledged in a single ACK. While toward the end 1368*2 bytes of data is acknowledged. A TCP segment typically contains 1368 bytes of data.

#### Question 12
Throughput = (TCP maximum receive window size)/RTT
