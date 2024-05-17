static void efx_set_multicast_list(struct net_device *net_dev)
{
	struct efx_nic *efx = netdev_priv(net_dev);
	struct netdev_hw_addr *ha;
	union efx_multicast_hash *mc_hash = &efx->multicast_hash;
	u32 crc;
	int bit;

	efx->promiscuous = !!(net_dev->flags & IFF_PROMISC);

	 
	if (efx->promiscuous || (net_dev->flags & IFF_ALLMULTI)) {
		memset(mc_hash, 0xff, sizeof(*mc_hash));
	} else {
		memset(mc_hash, 0x00, sizeof(*mc_hash));
		netdev_for_each_mc_addr(ha, net_dev) {
			crc = ether_crc_le(ETH_ALEN, ha->addr);
			bit = crc & (EFX_MCAST_HASH_ENTRIES - 1);
			set_bit_le(bit, mc_hash->byte);
		}

		 
		set_bit_le(0xff, mc_hash->byte);
	}

	if (efx->port_enabled)
		queue_work(efx->workqueue, &efx->mac_work);
	 
}
