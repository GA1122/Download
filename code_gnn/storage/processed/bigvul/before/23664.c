isdn_net_rmall(void)
{
	u_long flags;
	int ret;

	 
	spin_lock_irqsave(&dev->lock, flags);
	while (dev->netdev) {
		if (!dev->netdev->local->master) {
			 
			spin_unlock_irqrestore(&dev->lock, flags);
			if ((ret = isdn_net_realrm(dev->netdev, NULL))) {
				return ret;
			}
			spin_lock_irqsave(&dev->lock, flags);
		}
	}
	dev->netdev = NULL;
	spin_unlock_irqrestore(&dev->lock, flags);
	return 0;
}
