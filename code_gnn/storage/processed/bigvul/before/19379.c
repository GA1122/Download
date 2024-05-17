int efx_init_irq_moderation(struct efx_nic *efx, unsigned int tx_usecs,
			    unsigned int rx_usecs, bool rx_adaptive,
			    bool rx_may_override_tx)
{
	struct efx_channel *channel;
	unsigned tx_ticks = irq_mod_ticks(tx_usecs, EFX_IRQ_MOD_RESOLUTION);
	unsigned rx_ticks = irq_mod_ticks(rx_usecs, EFX_IRQ_MOD_RESOLUTION);

	EFX_ASSERT_RESET_SERIALISED(efx);

	if (tx_ticks > EFX_IRQ_MOD_MAX || rx_ticks > EFX_IRQ_MOD_MAX)
		return -EINVAL;

	if (tx_ticks != rx_ticks && efx->tx_channel_offset == 0 &&
	    !rx_may_override_tx) {
		netif_err(efx, drv, efx->net_dev, "Channels are shared. "
			  "RX and TX IRQ moderation must be equal\n");
		return -EINVAL;
	}

	efx->irq_rx_adaptive = rx_adaptive;
	efx->irq_rx_moderation = rx_ticks;
	efx_for_each_channel(channel, efx) {
		if (efx_channel_has_rx_queue(channel))
			channel->irq_moderation = rx_ticks;
		else if (efx_channel_has_tx_queues(channel))
			channel->irq_moderation = tx_ticks;
	}

	return 0;
}
