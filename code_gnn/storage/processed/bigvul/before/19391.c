static int efx_net_stop(struct net_device *net_dev)
{
	struct efx_nic *efx = netdev_priv(net_dev);

	netif_dbg(efx, ifdown, efx->net_dev, "closing on CPU %d\n",
		  raw_smp_processor_id());

	if (efx->state != STATE_DISABLED) {
		 
		efx_stop_all(efx);
		efx_fini_channels(efx);
		efx_init_channels(efx);
	}

	return 0;
}
