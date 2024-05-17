static int max_required_rx_slots(struct xenvif *vif)
{
	int max = DIV_ROUND_UP(vif->dev->mtu, PAGE_SIZE);

	if (vif->can_sg || vif->gso || vif->gso_prefix)
		max += MAX_SKB_FRAGS + 1;  

	return max;
}