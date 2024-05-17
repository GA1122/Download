static void efx_set_channels(struct efx_nic *efx)
{
	struct efx_channel *channel;
	struct efx_tx_queue *tx_queue;

	efx->tx_channel_offset =
		separate_tx_channels ? efx->n_channels - efx->n_tx_channels : 0;

	 
	efx_for_each_channel(channel, efx) {
		efx_for_each_channel_tx_queue(tx_queue, channel)
			tx_queue->queue -= (efx->tx_channel_offset *
					    EFX_TXQ_TYPES);
	}
}
