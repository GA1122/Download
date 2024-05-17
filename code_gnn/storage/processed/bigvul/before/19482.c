void efx_fini_tx_queue(struct efx_tx_queue *tx_queue)
{
	if (!tx_queue->initialised)
		return;

	netif_dbg(tx_queue->efx, drv, tx_queue->efx->net_dev,
		  "shutting down TX queue %d\n", tx_queue->queue);

	tx_queue->initialised = false;

	 
	efx_nic_fini_tx(tx_queue);

	efx_release_tx_buffers(tx_queue);

	 
	efx_fini_tso(tx_queue);
}
