rio_open (struct net_device *dev)
{
	struct netdev_private *np = netdev_priv(dev);
	long ioaddr = dev->base_addr;
	int i;
	u16 macctrl;

	i = request_irq (dev->irq, rio_interrupt, IRQF_SHARED, dev->name, dev);
	if (i)
		return i;

	 
	writew (GlobalReset | DMAReset | FIFOReset | NetworkReset | HostReset,
		ioaddr + ASICCtrl + 2);
	mdelay(10);

	 
	writel (readl (ioaddr + DebugCtrl) | 0x0230, ioaddr + DebugCtrl);

	 
	if (np->jumbo != 0)
		writew (MAX_JUMBO+14, ioaddr + MaxFrameSize);

	alloc_list (dev);

	 
	for (i = 0; i < 6; i++)
		writeb (dev->dev_addr[i], ioaddr + StationAddr0 + i);

	set_multicast (dev);
	if (np->coalesce) {
		writel (np->rx_coalesce | np->rx_timeout << 16,
			ioaddr + RxDMAIntCtrl);
	}
	 
	writeb (0x20, ioaddr + RxDMAPollPeriod);
	writeb (0xff, ioaddr + TxDMAPollPeriod);
	writeb (0x30, ioaddr + RxDMABurstThresh);
	writeb (0x30, ioaddr + RxDMAUrgentThresh);
	writel (0x0007ffff, ioaddr + RmonStatMask);
	 
	clear_stats (dev);

	 
	if (np->vlan) {
		 
		writel (readl(ioaddr + RxDMAIntCtrl) | 0x7 << 10,
			ioaddr + RxDMAIntCtrl);
		 
		writew (np->vlan, ioaddr + VLANId);
		 
		writel (0x8100 << 16 | np->vlan, ioaddr + VLANTag);
		 
		writel (readl (ioaddr + MACCtrl) | AutoVLANuntagging,
			ioaddr + MACCtrl);
	}

	init_timer (&np->timer);
	np->timer.expires = jiffies + 1*HZ;
	np->timer.data = (unsigned long) dev;
	np->timer.function = rio_timer;
	add_timer (&np->timer);

	 
	writel (readl (ioaddr + MACCtrl) | StatsEnable | RxEnable | TxEnable,
			ioaddr + MACCtrl);

	macctrl = 0;
	macctrl |= (np->vlan) ? AutoVLANuntagging : 0;
	macctrl |= (np->full_duplex) ? DuplexSelect : 0;
	macctrl |= (np->tx_flow) ? TxFlowControlEnable : 0;
	macctrl |= (np->rx_flow) ? RxFlowControlEnable : 0;
	writew(macctrl,	ioaddr + MACCtrl);

	netif_start_queue (dev);

	 
	EnableInt ();
	return 0;
}
