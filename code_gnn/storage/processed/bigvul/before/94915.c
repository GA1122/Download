int usbnet_set_settings (struct net_device *net, struct ethtool_cmd *cmd)
{
	struct usbnet *dev = netdev_priv(net);
	int retval;

	if (!dev->mii.mdio_write)
		return -EOPNOTSUPP;

	retval = mii_ethtool_sset(&dev->mii, cmd);

	 
	if (dev->driver_info->link_reset)
		dev->driver_info->link_reset(dev);

	 
	usbnet_update_max_qlen(dev);

	return retval;

}
