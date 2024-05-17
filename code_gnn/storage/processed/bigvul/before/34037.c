static void netbk_tx_err(struct xenvif *vif,
			 struct xen_netif_tx_request *txp, RING_IDX end)
{
	RING_IDX cons = vif->tx.req_cons;

	do {
		make_tx_response(vif, txp, XEN_NETIF_RSP_ERROR);
		if (cons >= end)
			break;
		txp = RING_GET_REQUEST(&vif->tx, cons++);
	} while (1);
	vif->tx.req_cons = cons;
	xen_netbk_check_rx_xenvif(vif);
 	xenvif_put(vif);
 }