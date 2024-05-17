static void ri_tasklet(unsigned long dev)
{
	struct net_device *_dev = (struct net_device *)dev;
	struct ifb_private *dp = netdev_priv(_dev);
	struct netdev_queue *txq;
	struct sk_buff *skb;

	txq = netdev_get_tx_queue(_dev, 0);
	if ((skb = skb_peek(&dp->tq)) == NULL) {
		if (__netif_tx_trylock(txq)) {
			skb_queue_splice_tail_init(&dp->rq, &dp->tq);
			__netif_tx_unlock(txq);
		} else {
			 
			goto resched;
		}
	}

	while ((skb = __skb_dequeue(&dp->tq)) != NULL) {
		u32 from = G_TC_FROM(skb->tc_verd);

		skb->tc_verd = 0;
		skb->tc_verd = SET_TC_NCLS(skb->tc_verd);

		u64_stats_update_begin(&dp->tsync);
		dp->tx_packets++;
		dp->tx_bytes += skb->len;
		u64_stats_update_end(&dp->tsync);

		rcu_read_lock();
		skb->dev = dev_get_by_index_rcu(&init_net, skb->skb_iif);
		if (!skb->dev) {
			rcu_read_unlock();
			dev_kfree_skb(skb);
			_dev->stats.tx_dropped++;
			if (skb_queue_len(&dp->tq) != 0)
				goto resched;
			break;
		}
		rcu_read_unlock();
		skb->skb_iif = _dev->ifindex;

		if (from & AT_EGRESS) {
			dev_queue_xmit(skb);
		} else if (from & AT_INGRESS) {
			skb_pull(skb, skb->dev->hard_header_len);
			netif_receive_skb(skb);
		} else
			BUG();
	}

	if (__netif_tx_trylock(txq)) {
		if ((skb = skb_peek(&dp->rq)) == NULL) {
			dp->tasklet_pending = 0;
			if (netif_queue_stopped(_dev))
				netif_wake_queue(_dev);
		} else {
			__netif_tx_unlock(txq);
			goto resched;
		}
		__netif_tx_unlock(txq);
	} else {
resched:
		dp->tasklet_pending = 1;
		tasklet_schedule(&dp->ifb_tasklet);
	}

}