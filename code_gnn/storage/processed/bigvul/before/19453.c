static int efx_ethtool_get_rxfh_indir(struct net_device *net_dev,
				      struct ethtool_rxfh_indir *indir)
{
	struct efx_nic *efx = netdev_priv(net_dev);
	size_t copy_size =
		min_t(size_t, indir->size, ARRAY_SIZE(efx->rx_indir_table));

	if (efx_nic_rev(efx) < EFX_REV_FALCON_B0)
		return -EOPNOTSUPP;

	indir->size = ARRAY_SIZE(efx->rx_indir_table);
	memcpy(indir->ring_index, efx->rx_indir_table,
	       copy_size * sizeof(indir->ring_index[0]));
	return 0;
}
