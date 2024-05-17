static void efx_start_all(struct efx_nic *efx)
{
	struct efx_channel *channel;

	EFX_ASSERT_RESET_SERIALISED(efx);

	 
	if (efx->port_enabled)
		return;
	if ((efx->state != STATE_RUNNING) && (efx->state != STATE_INIT))
		return;
	if (efx_dev_registered(efx) && !netif_running(efx->net_dev))
		return;

	 
	efx_start_port(efx);

	if (efx_dev_registered(efx) && netif_device_present(efx->net_dev))
		netif_tx_wake_all_queues(efx->net_dev);

	efx_for_each_channel(channel, efx)
		efx_start_channel(channel);

	if (efx->legacy_irq)
		efx->legacy_irq_enabled = true;
	efx_nic_enable_interrupts(efx);

	 
	efx_mcdi_mode_event(efx);
	if (efx->reset_pending)
		efx_mcdi_mode_poll(efx);

	 
	if (efx->type->monitor != NULL) {
		queue_delayed_work(efx->workqueue, &efx->monitor_work,
				   efx_monitor_interval);
	} else {
		mutex_lock(&efx->mac_lock);
		if (efx->phy_op->poll(efx))
			efx_link_status_changed(efx);
		mutex_unlock(&efx->mac_lock);
	}

	efx->type->start_stats(efx);
}
