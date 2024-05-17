set_multicast (struct net_device *dev)
{
	long ioaddr = dev->base_addr;
	u32 hash_table[2];
	u16 rx_mode = 0;
	struct netdev_private *np = netdev_priv(dev);

	hash_table[0] = hash_table[1] = 0;
	 
	hash_table[1] |= 0x02000000;
	if (dev->flags & IFF_PROMISC) {
		 
		rx_mode = ReceiveAllFrames;
	} else if ((dev->flags & IFF_ALLMULTI) ||
			(netdev_mc_count(dev) > multicast_filter_limit)) {
		 
		rx_mode = ReceiveBroadcast | ReceiveMulticast | ReceiveUnicast;
	} else if (!netdev_mc_empty(dev)) {
		struct netdev_hw_addr *ha;
		 
		rx_mode =
		    ReceiveBroadcast | ReceiveMulticastHash | ReceiveUnicast;
		netdev_for_each_mc_addr(ha, dev) {
			int bit, index = 0;
			int crc = ether_crc_le(ETH_ALEN, ha->addr);
			 
			for (bit = 0; bit < 6; bit++)
				if (crc & (1 << (31 - bit)))
					index |= (1 << bit);
			hash_table[index / 32] |= (1 << (index % 32));
		}
	} else {
		rx_mode = ReceiveBroadcast | ReceiveUnicast;
	}
	if (np->vlan) {
		 
		rx_mode |= ReceiveVLANMatch;
	}

	writel (hash_table[0], ioaddr + HashTable0);
	writel (hash_table[1], ioaddr + HashTable1);
	writew (rx_mode, ioaddr + ReceiveMode);
}
