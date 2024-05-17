static void efx_reset_work(struct work_struct *data)
{
	struct efx_nic *efx = container_of(data, struct efx_nic, reset_work);
	unsigned long pending = ACCESS_ONCE(efx->reset_pending);

	if (!pending)
		return;

	 
	if (efx->state != STATE_RUNNING) {
		netif_info(efx, drv, efx->net_dev,
			   "scheduled reset quenched. NIC not RUNNING\n");
		return;
	}

	rtnl_lock();
	(void)efx_reset(efx, fls(pending) - 1);
	rtnl_unlock();
}
