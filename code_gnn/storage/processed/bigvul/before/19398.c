static int efx_pm_freeze(struct device *dev)
{
	struct efx_nic *efx = pci_get_drvdata(to_pci_dev(dev));

	efx->state = STATE_FINI;

	netif_device_detach(efx->net_dev);

	efx_stop_all(efx);
	efx_fini_channels(efx);

	return 0;
}
