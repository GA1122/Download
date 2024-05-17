static int efx_probe_interrupts(struct efx_nic *efx)
{
	int max_channels =
		min_t(int, efx->type->phys_addr_channels, EFX_MAX_CHANNELS);
	int rc, i;

	if (efx->interrupt_mode == EFX_INT_MODE_MSIX) {
		struct msix_entry xentries[EFX_MAX_CHANNELS];
		int n_channels;

		n_channels = efx_wanted_channels();
		if (separate_tx_channels)
			n_channels *= 2;
		n_channels = min(n_channels, max_channels);

		for (i = 0; i < n_channels; i++)
			xentries[i].entry = i;
		rc = pci_enable_msix(efx->pci_dev, xentries, n_channels);
		if (rc > 0) {
			netif_err(efx, drv, efx->net_dev,
				  "WARNING: Insufficient MSI-X vectors"
				  " available (%d < %d).\n", rc, n_channels);
			netif_err(efx, drv, efx->net_dev,
				  "WARNING: Performance may be reduced.\n");
			EFX_BUG_ON_PARANOID(rc >= n_channels);
			n_channels = rc;
			rc = pci_enable_msix(efx->pci_dev, xentries,
					     n_channels);
		}

		if (rc == 0) {
			efx->n_channels = n_channels;
			if (separate_tx_channels) {
				efx->n_tx_channels =
					max(efx->n_channels / 2, 1U);
				efx->n_rx_channels =
					max(efx->n_channels -
					    efx->n_tx_channels, 1U);
			} else {
				efx->n_tx_channels = efx->n_channels;
				efx->n_rx_channels = efx->n_channels;
			}
			rc = efx_init_rx_cpu_rmap(efx, xentries);
			if (rc) {
				pci_disable_msix(efx->pci_dev);
				return rc;
			}
			for (i = 0; i < n_channels; i++)
				efx_get_channel(efx, i)->irq =
					xentries[i].vector;
		} else {
			 
			efx->interrupt_mode = EFX_INT_MODE_MSI;
			netif_err(efx, drv, efx->net_dev,
				  "could not enable MSI-X\n");
		}
	}

	 
	if (efx->interrupt_mode == EFX_INT_MODE_MSI) {
		efx->n_channels = 1;
		efx->n_rx_channels = 1;
		efx->n_tx_channels = 1;
		rc = pci_enable_msi(efx->pci_dev);
		if (rc == 0) {
			efx_get_channel(efx, 0)->irq = efx->pci_dev->irq;
		} else {
			netif_err(efx, drv, efx->net_dev,
				  "could not enable MSI\n");
			efx->interrupt_mode = EFX_INT_MODE_LEGACY;
		}
	}

	 
	if (efx->interrupt_mode == EFX_INT_MODE_LEGACY) {
		efx->n_channels = 1 + (separate_tx_channels ? 1 : 0);
		efx->n_rx_channels = 1;
		efx->n_tx_channels = 1;
		efx->legacy_irq = efx->pci_dev->irq;
	}

	return 0;
}
