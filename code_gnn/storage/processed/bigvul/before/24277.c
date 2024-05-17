static netdev_tx_t vlan_dev_hard_start_xmit(struct sk_buff *skb,
					    struct net_device *dev)
{
	struct vlan_ethhdr *veth = (struct vlan_ethhdr *)(skb->data);
	unsigned int len;
	int ret;

	 
	if (veth->h_vlan_proto != htons(ETH_P_8021Q) ||
	    vlan_dev_info(dev)->flags & VLAN_FLAG_REORDER_HDR) {
		u16 vlan_tci;
		vlan_tci = vlan_dev_info(dev)->vlan_id;
		vlan_tci |= vlan_dev_get_egress_qos_mask(dev, skb);
		skb = __vlan_hwaccel_put_tag(skb, vlan_tci);
	}

	skb_set_dev(skb, vlan_dev_info(dev)->real_dev);
	len = skb->len;
	ret = dev_queue_xmit(skb);

	if (likely(ret == NET_XMIT_SUCCESS || ret == NET_XMIT_CN)) {
		struct vlan_pcpu_stats *stats;

		stats = this_cpu_ptr(vlan_dev_info(dev)->vlan_pcpu_stats);
		u64_stats_update_begin(&stats->syncp);
		stats->tx_packets++;
		stats->tx_bytes += len;
		u64_stats_update_end(&stats->syncp);
	} else {
		this_cpu_inc(vlan_dev_info(dev)->vlan_pcpu_stats->tx_dropped);
	}

	return ret;
}