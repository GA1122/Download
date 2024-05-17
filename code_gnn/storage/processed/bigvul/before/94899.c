u32 usbnet_get_link (struct net_device *net)
{
	struct usbnet *dev = netdev_priv(net);

	 
	if (dev->driver_info->check_connect)
		return dev->driver_info->check_connect (dev) == 0;

	 
	if (dev->mii.mdio_read)
		return mii_link_ok(&dev->mii);

	 
	return ethtool_op_get_link(net);
}
