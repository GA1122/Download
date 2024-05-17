static int fwnet_finish_incoming_packet(struct net_device *net,
					struct sk_buff *skb, u16 source_node_id,
					bool is_broadcast, u16 ether_type)
{
	struct fwnet_device *dev;
	int status;
	__be64 guid;

	switch (ether_type) {
	case ETH_P_ARP:
	case ETH_P_IP:
#if IS_ENABLED(CONFIG_IPV6)
	case ETH_P_IPV6:
#endif
		break;
	default:
		goto err;
	}

	dev = netdev_priv(net);
	 
	skb->dev = net;
	skb->ip_summed = CHECKSUM_NONE;

	 
	guid = cpu_to_be64(dev->card->guid);
	if (dev_hard_header(skb, net, ether_type,
			   is_broadcast ? net->broadcast : net->dev_addr,
			   NULL, skb->len) >= 0) {
		struct fwnet_header *eth;
		u16 *rawp;
		__be16 protocol;

		skb_reset_mac_header(skb);
		skb_pull(skb, sizeof(*eth));
		eth = (struct fwnet_header *)skb_mac_header(skb);
		if (fwnet_hwaddr_is_multicast(eth->h_dest)) {
			if (memcmp(eth->h_dest, net->broadcast,
				   net->addr_len) == 0)
				skb->pkt_type = PACKET_BROADCAST;
#if 0
			else
				skb->pkt_type = PACKET_MULTICAST;
#endif
		} else {
			if (memcmp(eth->h_dest, net->dev_addr, net->addr_len))
				skb->pkt_type = PACKET_OTHERHOST;
		}
		if (ntohs(eth->h_proto) >= ETH_P_802_3_MIN) {
			protocol = eth->h_proto;
		} else {
			rawp = (u16 *)skb->data;
			if (*rawp == 0xffff)
				protocol = htons(ETH_P_802_3);
			else
				protocol = htons(ETH_P_802_2);
		}
		skb->protocol = protocol;
	}
	status = netif_rx(skb);
	if (status == NET_RX_DROP) {
		net->stats.rx_errors++;
		net->stats.rx_dropped++;
	} else {
		net->stats.rx_packets++;
		net->stats.rx_bytes += skb->len;
	}

	return 0;

 err:
	net->stats.rx_errors++;
	net->stats.rx_dropped++;

	dev_kfree_skb_any(skb);

	return -ENOENT;
}