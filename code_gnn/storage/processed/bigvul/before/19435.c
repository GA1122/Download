static void efx_start_channel(struct efx_channel *channel)
{
	struct efx_rx_queue *rx_queue;

	netif_dbg(channel->efx, ifup, channel->efx->net_dev,
		  "starting chan %d\n", channel->channel);

	 
	channel->work_pending = false;
	channel->enabled = true;
	smp_wmb();

	 
	efx_for_each_channel_rx_queue(rx_queue, channel)
		efx_fast_push_rx_descriptors(rx_queue);

	napi_enable(&channel->napi_str);
}
