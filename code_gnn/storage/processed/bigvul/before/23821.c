static int macvlan_queue_xmit(struct sk_buff *skb, struct net_device *dev)
{
	const struct macvlan_dev *vlan = netdev_priv(dev);
	const struct macvlan_port *port = vlan->port;
	const struct macvlan_dev *dest;
	__u8 ip_summed = skb->ip_summed;

	if (vlan->mode == MACVLAN_MODE_BRIDGE) {
		const struct ethhdr *eth = (void *)skb->data;
		skb->ip_summed = CHECKSUM_UNNECESSARY;

		 
		if (is_multicast_ether_addr(eth->h_dest)) {
			macvlan_broadcast(skb, port, dev, MACVLAN_MODE_BRIDGE);
			goto xmit_world;
		}

		dest = macvlan_hash_lookup(port, eth->h_dest);
		if (dest && dest->mode == MACVLAN_MODE_BRIDGE) {
			 
			vlan->forward(vlan->lowerdev, skb);

			return NET_XMIT_SUCCESS;
		}
	}

xmit_world:
	skb->ip_summed = ip_summed;
	skb_set_dev(skb, vlan->lowerdev);
	return dev_queue_xmit(skb);
}