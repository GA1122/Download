static int efx_pm_thaw(struct device *dev)
{
	struct efx_nic *efx = pci_get_drvdata(to_pci_dev(dev));

	efx->state = STATE_INIT;

	efx_init_channels(efx);

	mutex_lock(&efx->mac_lock);
	efx->phy_op->reconfigure(efx);
	mutex_unlock(&efx->mac_lock);

	efx_start_all(efx);

	netif_device_attach(efx->net_dev);

	efx->state = STATE_RUNNING;

	efx->type->resume_wol(efx);

	 
	queue_work(reset_workqueue, &efx->reset_work);

	return 0;
}
