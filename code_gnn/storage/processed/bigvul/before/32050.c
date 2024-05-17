	__acquires(RCU)
{
	struct net *net = seq_file_net(seq);
	loff_t off;
	struct net_device *dev;

	rcu_read_lock();
	if (!*pos)
		return SEQ_START_TOKEN;

	off = 1;
	for_each_netdev_rcu(net, dev)
		if (off++ == *pos)
			return dev;

	return NULL;
}