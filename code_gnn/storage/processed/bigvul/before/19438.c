static void efx_stop_port(struct efx_nic *efx)
{
	netif_dbg(efx, ifdown, efx->net_dev, "stop port\n");

	mutex_lock(&efx->mac_lock);
	efx->port_enabled = false;
	mutex_unlock(&efx->mac_lock);

	 
	if (efx_dev_registered(efx)) {
		netif_addr_lock_bh(efx->net_dev);
		netif_addr_unlock_bh(efx->net_dev);
	}
}
