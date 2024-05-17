static int efx_ethtool_set_rx_ntuple(struct net_device *net_dev,
				     struct ethtool_rx_ntuple *ntuple)
{
	struct efx_nic *efx = netdev_priv(net_dev);
	struct ethtool_tcpip4_spec *ip_entry = &ntuple->fs.h_u.tcp_ip4_spec;
	struct ethtool_tcpip4_spec *ip_mask = &ntuple->fs.m_u.tcp_ip4_spec;
	struct ethhdr *mac_entry = &ntuple->fs.h_u.ether_spec;
	struct ethhdr *mac_mask = &ntuple->fs.m_u.ether_spec;
	struct efx_filter_spec filter;
	int rc;

	 
	if (ntuple->fs.action < ETHTOOL_RXNTUPLE_ACTION_CLEAR ||
	    ntuple->fs.action >= (s32)efx->n_rx_channels)
		return -EINVAL;

	if (~ntuple->fs.data_mask)
		return -EINVAL;

	efx_filter_init_rx(&filter, EFX_FILTER_PRI_MANUAL, 0,
			   (ntuple->fs.action == ETHTOOL_RXNTUPLE_ACTION_DROP) ?
			   0xfff : ntuple->fs.action);

	switch (ntuple->fs.flow_type) {
	case TCP_V4_FLOW:
	case UDP_V4_FLOW: {
		u8 proto = (ntuple->fs.flow_type == TCP_V4_FLOW ?
			    IPPROTO_TCP : IPPROTO_UDP);

		 
		if (ip_mask->ip4dst | ip_mask->pdst)
			return -EINVAL;
		 
		if ((ip_mask->ip4src | ip_mask->psrc) &&
		    ((__force u32)~ip_mask->ip4src |
		     (__force u16)~ip_mask->psrc))
			return -EINVAL;
		 
		if ((u8)~ip_mask->tos | (u16)~ntuple->fs.vlan_tag_mask)
			return -EINVAL;

		if (!ip_mask->ip4src)
			rc = efx_filter_set_ipv4_full(&filter, proto,
						      ip_entry->ip4dst,
						      ip_entry->pdst,
						      ip_entry->ip4src,
						      ip_entry->psrc);
		else
			rc = efx_filter_set_ipv4_local(&filter, proto,
						       ip_entry->ip4dst,
						       ip_entry->pdst);
		if (rc)
			return rc;
		break;
	}

	case ETHER_FLOW:
		 
		if (!is_zero_ether_addr(mac_mask->h_dest))
			return -EINVAL;
		 
		if (ntuple->fs.vlan_tag_mask != 0xf000 &&
		    ntuple->fs.vlan_tag_mask != 0xffff)
			return -EINVAL;
		 
		if (!is_broadcast_ether_addr(mac_mask->h_source) ||
		    mac_mask->h_proto != htons(0xffff))
			return -EINVAL;

		rc = efx_filter_set_eth_local(
			&filter,
			(ntuple->fs.vlan_tag_mask == 0xf000) ?
			ntuple->fs.vlan_tag : EFX_FILTER_VID_UNSPEC,
			mac_entry->h_dest);
		if (rc)
			return rc;
		break;

	default:
		return -EINVAL;
	}

	if (ntuple->fs.action == ETHTOOL_RXNTUPLE_ACTION_CLEAR)
		return efx_filter_remove_filter(efx, &filter);

	rc = efx_filter_insert_filter(efx, &filter, true);
	return rc < 0 ? rc : 0;
}
