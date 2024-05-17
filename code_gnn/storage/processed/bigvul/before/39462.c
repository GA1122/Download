static void yam_arbitrate(struct net_device *dev)
{
	struct yam_port *yp = netdev_priv(dev);

	if (yp->magic != YAM_MAGIC || yp->tx_state != TX_OFF ||
	    skb_queue_empty(&yp->send_queue))
		return;
	 

	if (yp->dupmode) {
		 
		yam_start_tx(dev, yp);
		return;
	}
	if (yp->dcd) {
		 
		yp->slotcnt = yp->slot / 10;
		return;
	}
	 
	if ((--yp->slotcnt) > 0)
		return;

	yp->slotcnt = yp->slot / 10;

	 
	if ((prandom_u32() % 256) > yp->pers)
		return;

	yam_start_tx(dev, yp);
}
