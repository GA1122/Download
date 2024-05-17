static void efx_start_port(struct efx_nic *efx)
{
	netif_dbg(efx, ifup, efx->net_dev, "start port\n");
	BUG_ON(efx->port_enabled);

	mutex_lock(&efx->mac_lock);
	efx->port_enabled = true;

	 
	efx->type->push_multicast_hash(efx);
	efx->mac_op->reconfigure(efx);

	mutex_unlock(&efx->mac_lock);
}
