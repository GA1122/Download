tx_error (struct net_device *dev, int tx_status)
{
	struct netdev_private *np;
	long ioaddr = dev->base_addr;
	int frame_id;
	int i;

	np = netdev_priv(dev);

	frame_id = (tx_status & 0xffff0000);
	printk (KERN_ERR "%s: Transmit error, TxStatus %4.4x, FrameId %d.\n",
		dev->name, tx_status, frame_id);
	np->stats.tx_errors++;
	 
	if (tx_status & 0x10) {
		np->stats.tx_fifo_errors++;
		writew (readw (ioaddr + TxStartThresh) + 0x10,
			ioaddr + TxStartThresh);
		 
		writew (TxReset | DMAReset | FIFOReset | NetworkReset,
			ioaddr + ASICCtrl + 2);
		 
		for (i = 50; i > 0; i--) {
			if ((readw (ioaddr + ASICCtrl + 2) & ResetBusy) == 0)
				break;
			mdelay (1);
		}
		rio_free_tx (dev, 1);
		 
		writel (np->tx_ring_dma +
			np->old_tx * sizeof (struct netdev_desc),
			dev->base_addr + TFDListPtr0);
		writel (0, dev->base_addr + TFDListPtr1);

		 
	}
	 
	if (tx_status & 0x04) {
		np->stats.tx_fifo_errors++;
		 
		writew (TxReset | FIFOReset, ioaddr + ASICCtrl + 2);
		 
		for (i = 50; i > 0; i--) {
			if ((readw (ioaddr + ASICCtrl + 2) & ResetBusy) == 0)
				break;
			mdelay (1);
		}
		 
	}
	 
#ifdef ETHER_STATS
	if (tx_status & 0x08)
		np->stats.collisions16++;
#else
	if (tx_status & 0x08)
		np->stats.collisions++;
#endif
	 
	writel (readw (dev->base_addr + MACCtrl) | TxEnable, ioaddr + MACCtrl);
}