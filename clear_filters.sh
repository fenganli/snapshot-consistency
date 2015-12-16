sudo tc qdisc del dev eth4 root
sudo tc qdisc del dev eth4 ingress
sudo tc class del dev eth4 root
sudo tc filter del dev eth4 root
