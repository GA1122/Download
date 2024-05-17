static int efx_set_features(struct net_device *net_dev, u32 data)
{
	struct efx_nic *efx = netdev_priv(net_dev);

	 
	if (net_dev->features & ~data & NETIF_F_NTUPLE)
		efx_filter_clear_rx(efx, EFX_FILTER_PRI_MANUAL);

	return 0;
}
