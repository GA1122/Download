static netdev_tx_t tun_net_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct tun_struct *tun = netdev_priv(dev);

	tun_debug(KERN_INFO, tun, "tun_net_xmit %d\n", skb->len);

	 
	if (!tun->tfile)
		goto drop;

	 
	if (!check_filter(&tun->txflt, skb))
		goto drop;

	if (tun->socket.sk->sk_filter &&
	    sk_filter(tun->socket.sk, skb))
		goto drop;

	if (skb_queue_len(&tun->socket.sk->sk_receive_queue) >= dev->tx_queue_len) {
		if (!(tun->flags & TUN_ONE_QUEUE)) {
			 
			 
			netif_stop_queue(dev);

			 
			dev->stats.tx_fifo_errors++;
		} else {
			 
			goto drop;
		}
	}

	 
	skb_orphan(skb);

	 
	skb_queue_tail(&tun->socket.sk->sk_receive_queue, skb);

	 
	if (tun->flags & TUN_FASYNC)
		kill_fasync(&tun->fasync, SIGIO, POLL_IN);
	wake_up_interruptible_poll(&tun->wq.wait, POLLIN |
				   POLLRDNORM | POLLRDBAND);
	return NETDEV_TX_OK;

drop:
	dev->stats.tx_dropped++;
	kfree_skb(skb);
	return NETDEV_TX_OK;
}
