static struct ethhdr *fm10k_gre_is_nvgre(struct sk_buff *skb)
{
	struct fm10k_nvgre_hdr *nvgre_hdr;
	int hlen = ip_hdrlen(skb);

	 
	if (vlan_get_protocol(skb) != htons(ETH_P_IP))
		return NULL;

	 
	nvgre_hdr = (struct fm10k_nvgre_hdr *)(skb_network_header(skb) + hlen);

	 
	if (nvgre_hdr->flags & FM10K_NVGRE_RESERVED0_FLAGS)
		return NULL;

	 
	if (nvgre_hdr->flags & NVGRE_TNI)
		return (struct ethhdr *)(nvgre_hdr + 1);

	return (struct ethhdr *)(&nvgre_hdr->tni);
}
