isdn_net_realrm(isdn_net_dev * p, isdn_net_dev * q)
{
	u_long flags;

	if (isdn_net_device_started(p)) {
		return -EBUSY;
	}
#ifdef CONFIG_ISDN_X25
	if( p -> cprot && p -> cprot -> pops )
		p -> cprot -> pops -> proto_del ( p -> cprot );
#endif
	 
	isdn_net_rmallphone(p);
	 
	if (p->local->exclusive != -1)
		isdn_unexclusive_channel(p->local->pre_device, p->local->pre_channel);
	if (p->local->master) {
		 
		if (((isdn_net_local *) ISDN_MASTER_PRIV(p->local))->slave ==
		    p->dev)
			((isdn_net_local *)ISDN_MASTER_PRIV(p->local))->slave =
				p->local->slave;
	} else {
		 
		unregister_netdev(p->dev);
	}
	 
	spin_lock_irqsave(&dev->lock, flags);
	if (q)
		q->next = p->next;
	else
		dev->netdev = p->next;
	if (p->local->slave) {
		 
		char *slavename = p->local->slave->name;
		isdn_net_dev *n = dev->netdev;
		q = NULL;
		while (n) {
			if (!strcmp(n->dev->name, slavename)) {
				spin_unlock_irqrestore(&dev->lock, flags);
				isdn_net_realrm(n, q);
				spin_lock_irqsave(&dev->lock, flags);
				break;
			}
			q = n;
			n = (isdn_net_dev *)n->next;
		}
	}
	spin_unlock_irqrestore(&dev->lock, flags);
	 
	if (dev->netdev == NULL)
		isdn_timer_ctrl(ISDN_TIMER_NETHANGUP, 0);
	free_netdev(p->dev);
	kfree(p);

	return 0;
}
