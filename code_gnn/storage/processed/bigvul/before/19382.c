efx_init_rx_cpu_rmap(struct efx_nic *efx, struct msix_entry *xentries)
{
#ifdef CONFIG_RFS_ACCEL
	int i, rc;

	efx->net_dev->rx_cpu_rmap = alloc_irq_cpu_rmap(efx->n_rx_channels);
	if (!efx->net_dev->rx_cpu_rmap)
		return -ENOMEM;
	for (i = 0; i < efx->n_rx_channels; i++) {
		rc = irq_cpu_rmap_add(efx->net_dev->rx_cpu_rmap,
				      xentries[i].vector);
		if (rc) {
			free_irq_cpu_rmap(efx->net_dev->rx_cpu_rmap);
			efx->net_dev->rx_cpu_rmap = NULL;
			return rc;
		}
	}
#endif
	return 0;
}
