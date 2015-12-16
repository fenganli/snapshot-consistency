./clear_filters.sh
sudo tc qdisc add dev eth4 root handle 1: htb default 12
sudo tc class add dev eth4 parent 1: classid 1:12 htb rate 10000mbit ceil 10000mbit

sudo tc class add dev eth4 parent 1: classid 1:2 htb rate 400mbit ceil 400mbit
sudo tc qdisc add dev eth4 parent 1:2 handle 2: netem delay $1ms 20ms 25% distribution normal
sudo tc filter add dev eth4 protocol ip parent 1:0 prio 1 u32 match ip dst 10.10.1.1/32 flowid 1:2

sudo tc class add dev eth4 parent 1: classid 1:2 htb rate 400mbit ceil 400mbit
sudo tc qdisc add dev eth4 parent 1:2 handle 2: netem delay $1ms 20ms 25% distribution normal
sudo tc filter add dev eth4 protocol ip parent 1:0 prio 1 u32 match ip dst 10.10.1.3/32 flowid 1:2
