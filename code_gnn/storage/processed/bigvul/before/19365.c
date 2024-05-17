efx_alloc_channel(struct efx_nic *efx, int i, struct efx_channel *old_channel)
{
	struct efx_channel *channel;
	struct efx_rx_queue *rx_queue;
	struct efx_tx_queue *tx_queue;
	int j;

	if (old_channel) {
		channel = kmalloc(sizeof(*channel), GFP_KERNEL);
		if (!channel)
			return NULL;

		*channel = *old_channel;

		channel->napi_dev = NULL;
		memset(&channel->eventq, 0, sizeof(channel->eventq));

		rx_queue = &channel->rx_queue;
		rx_queue->buffer = NULL;
		memset(&rx_queue->rxd, 0, sizeof(rx_queue->rxd));

		for (j = 0; j < EFX_TXQ_TYPES; j++) {
			tx_queue = &channel->tx_queue[j];
			if (tx_queue->channel)
				tx_queue->channel = channel;
			tx_queue->buffer = NULL;
			memset(&tx_queue->txd, 0, sizeof(tx_queue->txd));
		}
	} else {
		channel = kzalloc(sizeof(*channel), GFP_KERNEL);
		if (!channel)
			return NULL;

		channel->efx = efx;
		channel->channel = i;

		for (j = 0; j < EFX_TXQ_TYPES; j++) {
			tx_queue = &channel->tx_queue[j];
			tx_queue->efx = efx;
			tx_queue->queue = i * EFX_TXQ_TYPES + j;
			tx_queue->channel = channel;
		}
	}

	rx_queue = &channel->rx_queue;
	rx_queue->efx = efx;
	setup_timer(&rx_queue->slow_fill, efx_rx_slow_fill,
		    (unsigned long)rx_queue);

	return channel;
}
