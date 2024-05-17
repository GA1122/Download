static struct macvtap_queue *macvtap_get_queue(struct net_device *dev,
					       struct sk_buff *skb)
{
	struct macvlan_dev *vlan = netdev_priv(dev);
	struct macvtap_queue *tap = NULL;
	int numvtaps = vlan->numvtaps;
	__u32 rxq;

	if (!numvtaps)
		goto out;

	 
	rxq = skb_get_rxhash(skb);
	if (rxq) {
		tap = rcu_dereference(vlan->taps[rxq % numvtaps]);
		if (tap)
			goto out;
	}

	if (likely(skb_rx_queue_recorded(skb))) {
		rxq = skb_get_rx_queue(skb);

		while (unlikely(rxq >= numvtaps))
			rxq -= numvtaps;

		tap = rcu_dereference(vlan->taps[rxq]);
		if (tap)
			goto out;
	}

	 
	for (rxq = 0; rxq < MAX_MACVTAP_QUEUES; rxq++) {
		tap = rcu_dereference(vlan->taps[rxq]);
		if (tap)
			break;
	}

out:
	return tap;
}