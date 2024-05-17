static void efx_mac_work(struct work_struct *data)
{
	struct efx_nic *efx = container_of(data, struct efx_nic, mac_work);

	mutex_lock(&efx->mac_lock);
	if (efx->port_enabled) {
		efx->type->push_multicast_hash(efx);
		efx->mac_op->reconfigure(efx);
	}
	mutex_unlock(&efx->mac_lock);
}
