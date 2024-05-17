static void efx_set_channel_names(struct efx_nic *efx)
{
	struct efx_channel *channel;
	const char *type = "";
	int number;

	efx_for_each_channel(channel, efx) {
		number = channel->channel;
		if (efx->n_channels > efx->n_rx_channels) {
			if (channel->channel < efx->n_rx_channels) {
				type = "-rx";
			} else {
				type = "-tx";
				number -= efx->n_rx_channels;
			}
		}
		snprintf(efx->channel_name[channel->channel],
			 sizeof(efx->channel_name[0]),
			 "%s%s-%d", efx->name, type, number);
	}
}
