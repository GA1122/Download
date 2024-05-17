int dev_hard_start_xmit(struct sk_buff *skb, struct net_device *dev,
			struct netdev_queue *txq)
{
	const struct net_device_ops *ops = dev->netdev_ops;
	int rc = NETDEV_TX_OK;

	if (likely(!skb->next)) {
		if (!list_empty(&ptype_all))
			dev_queue_xmit_nit(skb, dev);

		if (netif_needs_gso(dev, skb)) {
			if (unlikely(dev_gso_segment(skb)))
				goto out_kfree_skb;
			if (skb->next)
				goto gso;
		}

		 
		if (dev->priv_flags & IFF_XMIT_DST_RELEASE)
			skb_dst_drop(skb);

		rc = ops->ndo_start_xmit(skb, dev);
		if (rc == NETDEV_TX_OK)
			txq_trans_update(txq);
		 
		return rc;
	}

gso:
	do {
		struct sk_buff *nskb = skb->next;

		skb->next = nskb->next;
		nskb->next = NULL;

		 
		if (dev->priv_flags & IFF_XMIT_DST_RELEASE)
			skb_dst_drop(nskb);

		rc = ops->ndo_start_xmit(nskb, dev);
		if (unlikely(rc != NETDEV_TX_OK)) {
			if (rc & ~NETDEV_TX_MASK)
				goto out_kfree_gso_skb;
			nskb->next = skb->next;
			skb->next = nskb;
			return rc;
		}
		txq_trans_update(txq);
		if (unlikely(netif_tx_queue_stopped(txq) && skb->next))
			return NETDEV_TX_BUSY;
	} while (skb->next);

out_kfree_gso_skb:
	if (likely(skb->next == NULL))
		skb->destructor = DEV_GSO_CB(skb)->destructor;
out_kfree_skb:
	kfree_skb(skb);
	return rc;
}