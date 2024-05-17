int xen_netbk_must_stop_queue(struct xenvif *vif)
{
	if (!xen_netbk_rx_ring_full(vif))
		return 0;

	vif->rx.sring->req_event = vif->rx_req_cons_peek +
		max_required_rx_slots(vif);
	mb();  

	return xen_netbk_rx_ring_full(vif);
}