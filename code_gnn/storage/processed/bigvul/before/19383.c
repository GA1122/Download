static int efx_init_struct(struct efx_nic *efx, const struct efx_nic_type *type,
			   struct pci_dev *pci_dev, struct net_device *net_dev)
{
	int i;

	 
	memset(efx, 0, sizeof(*efx));
	spin_lock_init(&efx->biu_lock);
#ifdef CONFIG_SFC_MTD
	INIT_LIST_HEAD(&efx->mtd_list);
#endif
	INIT_WORK(&efx->reset_work, efx_reset_work);
	INIT_DELAYED_WORK(&efx->monitor_work, efx_monitor);
	efx->pci_dev = pci_dev;
	efx->msg_enable = debug;
	efx->state = STATE_INIT;
	strlcpy(efx->name, pci_name(pci_dev), sizeof(efx->name));

	efx->net_dev = net_dev;
	spin_lock_init(&efx->stats_lock);
	mutex_init(&efx->mac_lock);
	efx->mac_op = type->default_mac_ops;
	efx->phy_op = &efx_dummy_phy_operations;
	efx->mdio.dev = net_dev;
	INIT_WORK(&efx->mac_work, efx_mac_work);

	for (i = 0; i < EFX_MAX_CHANNELS; i++) {
		efx->channel[i] = efx_alloc_channel(efx, i, NULL);
		if (!efx->channel[i])
			goto fail;
	}

	efx->type = type;

	EFX_BUG_ON_PARANOID(efx->type->phys_addr_channels > EFX_MAX_CHANNELS);

	 
	efx->interrupt_mode = max(efx->type->max_interrupt_mode,
				  interrupt_mode);

	 
	snprintf(efx->workqueue_name, sizeof(efx->workqueue_name), "sfc%s",
		 pci_name(pci_dev));
	efx->workqueue = create_singlethread_workqueue(efx->workqueue_name);
	if (!efx->workqueue)
		goto fail;

	return 0;

fail:
	efx_fini_struct(efx);
	return -ENOMEM;
}
