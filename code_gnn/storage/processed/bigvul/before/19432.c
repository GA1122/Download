static int efx_set_mac_address(struct net_device *net_dev, void *data)
{
	struct efx_nic *efx = netdev_priv(net_dev);
	struct sockaddr *addr = data;
	char *new_addr = addr->sa_data;

	EFX_ASSERT_RESET_SERIALISED(efx);

	if (!is_valid_ether_addr(new_addr)) {
		netif_err(efx, drv, efx->net_dev,
			  "invalid ethernet MAC address requested: %pM\n",
			  new_addr);
		return -EINVAL;
	}

	memcpy(net_dev->dev_addr, new_addr, net_dev->addr_len);

	 
	mutex_lock(&efx->mac_lock);
	efx->mac_op->reconfigure(efx);
	mutex_unlock(&efx->mac_lock);

	return 0;
}
