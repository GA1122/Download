static void efx_stop_channel(struct efx_channel *channel)
{
	if (!channel->enabled)
		return;

	netif_dbg(channel->efx, ifdown, channel->efx->net_dev,
		  "stop chan %d\n", channel->channel);

	channel->enabled = false;
	napi_disable(&channel->napi_str);
}
