static netdev_tx_t airo_start_xmit(struct sk_buff *skb,
					 struct net_device *dev)
{
	s16 len;
	int i, j;
	struct airo_info *priv = dev->ml_priv;
	u32 *fids = priv->fids;

	if ( skb == NULL ) {
		airo_print_err(dev->name, "%s: skb == NULL!", __func__);
		return NETDEV_TX_OK;
	}

	 
	for( i = 0; i < MAX_FIDS / 2 && (fids[i] & 0xffff0000); i++ );
	for( j = i + 1; j < MAX_FIDS / 2 && (fids[j] & 0xffff0000); j++ );

	if ( j >= MAX_FIDS / 2 ) {
		netif_stop_queue(dev);

		if (i == MAX_FIDS / 2) {
			dev->stats.tx_fifo_errors++;
			return NETDEV_TX_BUSY;
		}
	}
	 
	len = ETH_ZLEN < skb->len ? skb->len : ETH_ZLEN;
         
	fids[i] |= (len << 16);
	priv->xmit.skb = skb;
	priv->xmit.fid = i;
	if (down_trylock(&priv->sem) != 0) {
		set_bit(FLAG_PENDING_XMIT, &priv->flags);
		netif_stop_queue(dev);
		set_bit(JOB_XMIT, &priv->jobs);
		wake_up_interruptible(&priv->thr_wait);
	} else
		airo_end_xmit(dev);
	return NETDEV_TX_OK;
}