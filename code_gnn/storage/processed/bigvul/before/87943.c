static inline void fm10k_rx_checksum(struct fm10k_ring *ring,
				     union fm10k_rx_desc *rx_desc,
				     struct sk_buff *skb)
{
	skb_checksum_none_assert(skb);

	 
	if (!(ring->netdev->features & NETIF_F_RXCSUM))
		return;

	 
	if (fm10k_test_staterr(rx_desc,
			       FM10K_RXD_STATUS_L4E |
			       FM10K_RXD_STATUS_L4E2 |
			       FM10K_RXD_STATUS_IPE |
			       FM10K_RXD_STATUS_IPE2)) {
		ring->rx_stats.csum_err++;
		return;
	}

	 
	if (fm10k_test_staterr(rx_desc, FM10K_RXD_STATUS_L4CS2))
		skb->encapsulation = true;
	else if (!fm10k_test_staterr(rx_desc, FM10K_RXD_STATUS_L4CS))
		return;

	skb->ip_summed = CHECKSUM_UNNECESSARY;

	ring->rx_stats.csum_good++;
}
