static netdev_features_t atl2_fix_features(struct net_device *netdev,
	netdev_features_t features)
{
	 
	if (features & NETIF_F_HW_VLAN_CTAG_RX)
		features |= NETIF_F_HW_VLAN_CTAG_TX;
	else
		features &= ~NETIF_F_HW_VLAN_CTAG_TX;

	return features;
}
