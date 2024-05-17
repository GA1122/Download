static netdev_tx_t bnep_net_xmit(struct sk_buff *skb,
				 struct net_device *dev)
{
	struct bnep_session *s = netdev_priv(dev);
	struct sock *sk = s->sock->sk;

	BT_DBG("skb %p, dev %p", skb, dev);

#ifdef CONFIG_BT_BNEP_MC_FILTER
	if (bnep_net_mc_filter(skb, s)) {
		kfree_skb(skb);
		return NETDEV_TX_OK;
	}
#endif

#ifdef CONFIG_BT_BNEP_PROTO_FILTER
	if (bnep_net_proto_filter(skb, s)) {
		kfree_skb(skb);
		return NETDEV_TX_OK;
	}
#endif

	 
	dev->trans_start = jiffies;
	skb_queue_tail(&sk->sk_write_queue, skb);
	wake_up_interruptible(sk_sleep(sk));

	if (skb_queue_len(&sk->sk_write_queue) >= BNEP_TX_QUEUE_LEN) {
		BT_DBG("tx queue is full");

		 
		netif_stop_queue(dev);
	}

	return NETDEV_TX_OK;
}
