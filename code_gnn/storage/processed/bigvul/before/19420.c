static void efx_remove_interrupts(struct efx_nic *efx)
{
	struct efx_channel *channel;

	 
	efx_for_each_channel(channel, efx)
		channel->irq = 0;
	pci_disable_msi(efx->pci_dev);
	pci_disable_msix(efx->pci_dev);

	 
	efx->legacy_irq = 0;
}
