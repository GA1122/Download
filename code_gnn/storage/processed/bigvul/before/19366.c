static int efx_change_mtu(struct net_device *net_dev, int new_mtu)
{
	struct efx_nic *efx = netdev_priv(net_dev);
	int rc = 0;

	EFX_ASSERT_RESET_SERIALISED(efx);

	if (new_mtu > EFX_MAX_MTU)
		return -EINVAL;

	efx_stop_all(efx);

	netif_dbg(efx, drv, efx->net_dev, "changing MTU to %d\n", new_mtu);

	efx_fini_channels(efx);

	mutex_lock(&efx->mac_lock);
	 
	net_dev->mtu = new_mtu;
	efx->mac_op->reconfigure(efx);
	mutex_unlock(&efx->mac_lock);

	efx_init_channels(efx);

	efx_start_all(efx);
	return rc;
}
