static void rtl8150_set_multicast(struct net_device *netdev)
{
	rtl8150_t *dev = netdev_priv(netdev);
	u16 rx_creg = 0x9e;

	netif_stop_queue(netdev);
	if (netdev->flags & IFF_PROMISC) {
		rx_creg |= 0x0001;
		dev_info(&netdev->dev, "%s: promiscuous mode\n", netdev->name);
	} else if (!netdev_mc_empty(netdev) ||
		   (netdev->flags & IFF_ALLMULTI)) {
		rx_creg &= 0xfffe;
		rx_creg |= 0x0002;
		dev_info(&netdev->dev, "%s: allmulti set\n", netdev->name);
	} else {
		 
		rx_creg &= 0x00fc;
	}
	async_set_registers(dev, RCR, sizeof(rx_creg), rx_creg);
	netif_wake_queue(netdev);
}