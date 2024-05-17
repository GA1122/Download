static void efx_flush_all(struct efx_nic *efx)
{
	 
	cancel_delayed_work_sync(&efx->monitor_work);
	 
	cancel_work_sync(&efx->mac_work);
}
