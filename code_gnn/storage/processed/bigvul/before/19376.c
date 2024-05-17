static void efx_init_channels(struct efx_nic *efx)
{
	struct efx_tx_queue *tx_queue;
	struct efx_rx_queue *rx_queue;
	struct efx_channel *channel;

	 
	efx->rx_buffer_len = (max(EFX_PAGE_IP_ALIGN, NET_IP_ALIGN) +
			      EFX_MAX_FRAME_LEN(efx->net_dev->mtu) +
			      efx->type->rx_buffer_hash_size +
			      efx->type->rx_buffer_padding);
	efx->rx_buffer_order = get_order(efx->rx_buffer_len +
					 sizeof(struct efx_rx_page_state));

	 
	efx_for_each_channel(channel, efx) {
		netif_dbg(channel->efx, drv, channel->efx->net_dev,
			  "init chan %d\n", channel->channel);

		efx_init_eventq(channel);

		efx_for_each_channel_tx_queue(tx_queue, channel)
			efx_init_tx_queue(tx_queue);

		 
		efx_rx_strategy(channel);

		efx_for_each_channel_rx_queue(rx_queue, channel)
			efx_init_rx_queue(rx_queue);

		WARN_ON(channel->rx_pkt != NULL);
		efx_rx_strategy(channel);
	}
}
