static int efx_process_channel(struct efx_channel *channel, int budget)
{
	struct efx_nic *efx = channel->efx;
	int spent;

	if (unlikely(efx->reset_pending || !channel->enabled))
		return 0;

	spent = efx_nic_process_eventq(channel, budget);
	if (spent == 0)
		return 0;

	 
	if (channel->rx_pkt) {
		__efx_rx_packet(channel, channel->rx_pkt,
				channel->rx_pkt_csummed);
		channel->rx_pkt = NULL;
	}

	efx_rx_strategy(channel);

	efx_fast_push_rx_descriptors(efx_channel_get_rx_queue(channel));

	return spent;
}
