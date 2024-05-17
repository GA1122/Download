void efx_get_irq_moderation(struct efx_nic *efx, unsigned int *tx_usecs,
			    unsigned int *rx_usecs, bool *rx_adaptive)
{
	*rx_adaptive = efx->irq_rx_adaptive;
	*rx_usecs = efx->irq_rx_moderation * EFX_IRQ_MOD_RESOLUTION;

	 
	if (efx->tx_channel_offset == 0)
		*tx_usecs = *rx_usecs;
	else
		*tx_usecs =
			efx->channel[efx->tx_channel_offset]->irq_moderation *
			EFX_IRQ_MOD_RESOLUTION;
}
