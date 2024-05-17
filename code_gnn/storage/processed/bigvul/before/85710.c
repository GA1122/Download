static void hns_nic_rx_checksum(struct hns_nic_ring_data *ring_data,
				struct sk_buff *skb, u32 flag)
{
	struct net_device *netdev = ring_data->napi.dev;
	u32 l3id;
	u32 l4id;

	 
	if (unlikely(!(netdev->features & NETIF_F_RXCSUM)))
		return;

	 

	l3id = hnae_get_field(flag, HNS_RXD_L3ID_M, HNS_RXD_L3ID_S);
	l4id = hnae_get_field(flag, HNS_RXD_L4ID_M, HNS_RXD_L4ID_S);

	 
	if ((l3id != HNS_RX_FLAG_L3ID_IPV4) && (l3id != HNS_RX_FLAG_L3ID_IPV6))
		return;

	 
	if (unlikely(hnae_get_bit(flag, HNS_RXD_L3E_B)))
		return;

	 
	if (unlikely(hnae_get_bit(flag, HNS_RXD_FRAG_B)))
		return;

	 
	if ((l4id != HNS_RX_FLAG_L4ID_TCP) &&
	    (l4id != HNS_RX_FLAG_L4ID_UDP) &&
	    (l4id != HNS_RX_FLAG_L4ID_SCTP))
		return;

	 
	if (unlikely(hnae_get_bit(flag, HNS_RXD_L4E_B)))
		return;

	 
	skb->ip_summed = CHECKSUM_UNNECESSARY;
}
