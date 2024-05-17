static int __devinit efx_pci_probe(struct pci_dev *pci_dev,
				   const struct pci_device_id *entry)
{
	const struct efx_nic_type *type = (const struct efx_nic_type *) entry->driver_data;
	struct net_device *net_dev;
	struct efx_nic *efx;
	int i, rc;

	 
	net_dev = alloc_etherdev_mqs(sizeof(*efx), EFX_MAX_CORE_TX_QUEUES,
				     EFX_MAX_RX_QUEUES);
	if (!net_dev)
		return -ENOMEM;
	net_dev->features |= (type->offload_features | NETIF_F_SG |
			      NETIF_F_HIGHDMA | NETIF_F_TSO |
			      NETIF_F_RXCSUM);
	if (type->offload_features & NETIF_F_V6_CSUM)
		net_dev->features |= NETIF_F_TSO6;
	 
	net_dev->vlan_features |= (NETIF_F_ALL_CSUM | NETIF_F_SG |
				   NETIF_F_HIGHDMA | NETIF_F_ALL_TSO |
				   NETIF_F_RXCSUM);
	 
	net_dev->hw_features = net_dev->features & ~NETIF_F_HIGHDMA;
	efx = netdev_priv(net_dev);
	pci_set_drvdata(pci_dev, efx);
	SET_NETDEV_DEV(net_dev, &pci_dev->dev);
	rc = efx_init_struct(efx, type, pci_dev, net_dev);
	if (rc)
		goto fail1;

	netif_info(efx, probe, efx->net_dev,
		   "Solarflare NIC detected\n");

	 
	rc = efx_init_io(efx);
	if (rc)
		goto fail2;

	 
	for (i = 0; i < 5; i++) {
		rc = efx_pci_probe_main(efx);

		 
		cancel_work_sync(&efx->reset_work);

		if (rc == 0) {
			if (efx->reset_pending) {
				 
				efx_pci_remove_main(efx);
				rc = -EIO;
			} else {
				break;
			}
		}

		 
		if (efx->reset_pending &
		    ~(1 << RESET_TYPE_INVISIBLE | 1 << RESET_TYPE_ALL) ||
		    !efx->reset_pending)
			goto fail3;

		efx->reset_pending = 0;
	}

	if (rc) {
		netif_err(efx, probe, efx->net_dev, "Could not reset NIC\n");
		goto fail4;
	}

	 
	efx->state = STATE_RUNNING;

	rc = efx_register_netdev(efx);
	if (rc)
		goto fail5;

	netif_dbg(efx, probe, efx->net_dev, "initialisation successful\n");

	rtnl_lock();
	efx_mtd_probe(efx);  
	rtnl_unlock();
	return 0;

 fail5:
	efx_pci_remove_main(efx);
 fail4:
 fail3:
	efx_fini_io(efx);
 fail2:
	efx_fini_struct(efx);
 fail1:
	WARN_ON(rc > 0);
	netif_dbg(efx, drv, efx->net_dev, "initialisation failed. rc=%d\n", rc);
	free_netdev(net_dev);
	return rc;
}
