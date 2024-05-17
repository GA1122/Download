static void tx_add_credit(struct xenvif *vif)
{
	unsigned long max_burst, max_credit;

	 
	max_burst = RING_GET_REQUEST(&vif->tx, vif->tx.req_cons)->size;
	max_burst = min(max_burst, 131072UL);
	max_burst = max(max_burst, vif->credit_bytes);

	 
	max_credit = vif->remaining_credit + vif->credit_bytes;
	if (max_credit < vif->remaining_credit)
		max_credit = ULONG_MAX;  

	vif->remaining_credit = min(max_credit, max_burst);
}
