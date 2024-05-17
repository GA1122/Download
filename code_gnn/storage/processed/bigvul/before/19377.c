static void efx_init_eventq(struct efx_channel *channel)
{
	netif_dbg(channel->efx, drv, channel->efx->net_dev,
		  "chan %d init event queue\n", channel->channel);

	channel->eventq_read_ptr = 0;

	efx_nic_init_eventq(channel);
}
