static struct rtnl_link_stats64 *macvlan_dev_get_stats64(struct net_device *dev,
							 struct rtnl_link_stats64 *stats)
{
	struct macvlan_dev *vlan = netdev_priv(dev);

	if (vlan->pcpu_stats) {
		struct macvlan_pcpu_stats *p;
		u64 rx_packets, rx_bytes, rx_multicast, tx_packets, tx_bytes;
		u32 rx_errors = 0, tx_dropped = 0;
		unsigned int start;
		int i;

		for_each_possible_cpu(i) {
			p = per_cpu_ptr(vlan->pcpu_stats, i);
			do {
				start = u64_stats_fetch_begin_bh(&p->syncp);
				rx_packets	= p->rx_packets;
				rx_bytes	= p->rx_bytes;
				rx_multicast	= p->rx_multicast;
				tx_packets	= p->tx_packets;
				tx_bytes	= p->tx_bytes;
			} while (u64_stats_fetch_retry_bh(&p->syncp, start));

			stats->rx_packets	+= rx_packets;
			stats->rx_bytes		+= rx_bytes;
			stats->multicast	+= rx_multicast;
			stats->tx_packets	+= tx_packets;
			stats->tx_bytes		+= tx_bytes;
			 
			rx_errors	+= p->rx_errors;
			tx_dropped	+= p->tx_dropped;
		}
		stats->rx_errors	= rx_errors;
		stats->rx_dropped	= rx_errors;
		stats->tx_dropped	= tx_dropped;
	}
	return stats;
}