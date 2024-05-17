static struct rtnl_link_stats64 *vlan_dev_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *stats)
{

	if (vlan_dev_info(dev)->vlan_pcpu_stats) {
		struct vlan_pcpu_stats *p;
		u32 rx_errors = 0, tx_dropped = 0;
		int i;

		for_each_possible_cpu(i) {
			u64 rxpackets, rxbytes, rxmulticast, txpackets, txbytes;
			unsigned int start;

			p = per_cpu_ptr(vlan_dev_info(dev)->vlan_pcpu_stats, i);
			do {
				start = u64_stats_fetch_begin_bh(&p->syncp);
				rxpackets	= p->rx_packets;
				rxbytes		= p->rx_bytes;
				rxmulticast	= p->rx_multicast;
				txpackets	= p->tx_packets;
				txbytes		= p->tx_bytes;
			} while (u64_stats_fetch_retry_bh(&p->syncp, start));

			stats->rx_packets	+= rxpackets;
			stats->rx_bytes		+= rxbytes;
			stats->multicast	+= rxmulticast;
			stats->tx_packets	+= txpackets;
			stats->tx_bytes		+= txbytes;
			 
			rx_errors	+= p->rx_errors;
			tx_dropped	+= p->tx_dropped;
		}
		stats->rx_errors  = rx_errors;
		stats->tx_dropped = tx_dropped;
	}
	return stats;
}