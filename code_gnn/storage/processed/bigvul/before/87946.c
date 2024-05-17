static bool fm10k_set_qos_queues(struct fm10k_intfc *interface)
{
	struct net_device *dev = interface->netdev;
	struct fm10k_ring_feature *f;
	int rss_i, i;
	int pcs;

	 
	pcs = netdev_get_num_tc(dev);

	if (pcs <= 1)
		return false;

	 
	f = &interface->ring_feature[RING_F_QOS];
	f->indices = pcs;
	f->mask = BIT(fls(pcs - 1)) - 1;

	 
	rss_i = interface->hw.mac.max_queues / pcs;
	rss_i = BIT(fls(rss_i) - 1);

	 
	f = &interface->ring_feature[RING_F_RSS];
	rss_i = min_t(u16, rss_i, f->limit);
	f->indices = rss_i;
	f->mask = BIT(fls(rss_i - 1)) - 1;

	 
	for (i = 0; i < pcs; i++)
		netdev_set_tc_queue(dev, i, rss_i, rss_i * i);

	interface->num_rx_queues = rss_i * pcs;
	interface->num_tx_queues = rss_i * pcs;

	return true;
}