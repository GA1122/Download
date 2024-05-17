efx_realloc_channels(struct efx_nic *efx, u32 rxq_entries, u32 txq_entries)
{
	struct efx_channel *other_channel[EFX_MAX_CHANNELS], *channel;
	u32 old_rxq_entries, old_txq_entries;
	unsigned i;
	int rc;

	efx_stop_all(efx);
	efx_fini_channels(efx);

	 
	memset(other_channel, 0, sizeof(other_channel));
	for (i = 0; i < efx->n_channels; i++) {
		channel = efx_alloc_channel(efx, i, efx->channel[i]);
		if (!channel) {
			rc = -ENOMEM;
			goto out;
		}
		other_channel[i] = channel;
	}

	 
	old_rxq_entries = efx->rxq_entries;
	old_txq_entries = efx->txq_entries;
	efx->rxq_entries = rxq_entries;
	efx->txq_entries = txq_entries;
	for (i = 0; i < efx->n_channels; i++) {
		channel = efx->channel[i];
		efx->channel[i] = other_channel[i];
		other_channel[i] = channel;
	}

	rc = efx_probe_channels(efx);
	if (rc)
		goto rollback;

	efx_init_napi(efx);

	 
	for (i = 0; i < efx->n_channels; i++) {
		efx_fini_napi_channel(other_channel[i]);
		efx_remove_channel(other_channel[i]);
	}
out:
	 
	for (i = 0; i < efx->n_channels; i++)
		kfree(other_channel[i]);

	efx_init_channels(efx);
	efx_start_all(efx);
	return rc;

rollback:
	 
	efx->rxq_entries = old_rxq_entries;
	efx->txq_entries = old_txq_entries;
	for (i = 0; i < efx->n_channels; i++) {
		channel = efx->channel[i];
		efx->channel[i] = other_channel[i];
		other_channel[i] = channel;
	}
	goto out;
}
