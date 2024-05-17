isdn_net_xmit(struct net_device *ndev, struct sk_buff *skb)
{
	isdn_net_dev *nd;
	isdn_net_local *slp;
	isdn_net_local *lp = netdev_priv(ndev);
	int retv = NETDEV_TX_OK;

	if (((isdn_net_local *) netdev_priv(ndev))->master) {
		printk("isdn BUG at %s:%d!\n", __FILE__, __LINE__);
		dev_kfree_skb(skb);
		return NETDEV_TX_OK;
	}

	 
#ifdef CONFIG_ISDN_PPP
	if (lp->p_encap == ISDN_NET_ENCAP_SYNCPPP) {
		return isdn_ppp_xmit(skb, ndev);
	}
#endif
	nd = ((isdn_net_local *) netdev_priv(ndev))->netdev;
	lp = isdn_net_get_locked_lp(nd);
	if (!lp) {
		printk(KERN_WARNING "%s: all channels busy - requeuing!\n", ndev->name);
		return NETDEV_TX_BUSY;
	}
	 

	 
	lp->huptimer = 0;  
	isdn_net_writebuf_skb(lp, skb);
	spin_unlock_bh(&lp->xmit_lock);

	 
	if (lp->cps > lp->triggercps) {
		if (lp->slave) {
			if (!lp->sqfull) {
				 
				lp->sqfull = 1;
				lp->sqfull_stamp = jiffies;
			} else {
				 
				if (time_after(jiffies, lp->sqfull_stamp + lp->slavedelay)) {
					slp = ISDN_SLAVE_PRIV(lp);
					if (!(slp->flags & ISDN_NET_CONNECTED)) {
						isdn_net_force_dial_lp(ISDN_SLAVE_PRIV(lp));
					}
				}
			}
		}
	} else {
		if (lp->sqfull && time_after(jiffies, lp->sqfull_stamp + lp->slavedelay + (10 * HZ))) {
			lp->sqfull = 0;
		}
		 
		nd->queue = nd->local;
	}

	return retv;

}
