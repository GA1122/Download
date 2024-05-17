static void efx_pci_remove(struct pci_dev *pci_dev)
{
	struct efx_nic *efx;

	efx = pci_get_drvdata(pci_dev);
	if (!efx)
		return;

	 
	rtnl_lock();
	efx->state = STATE_FINI;
	dev_close(efx->net_dev);

	 
	rtnl_unlock();

	efx_unregister_netdev(efx);

	efx_mtd_remove(efx);

	 
	cancel_work_sync(&efx->reset_work);

	efx_pci_remove_main(efx);

	efx_fini_io(efx);
	netif_dbg(efx, drv, efx->net_dev, "shutdown successful\n");

	pci_set_drvdata(pci_dev, NULL);
	efx_fini_struct(efx);
	free_netdev(efx->net_dev);
};
