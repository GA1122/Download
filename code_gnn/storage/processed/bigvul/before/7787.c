static void h2_update_poll(struct conn_stream *cs)
{
	struct h2s *h2s = cs->ctx;

	if (!h2s)
		return;

	 

	if (cs->flags & CS_FL_DATA_RD_ENA) {
		 
		h2s->h2c->flags &= ~H2_CF_DEM_SFULL;
		if (h2s->h2c->dsi == h2s->id) {
			conn_xprt_want_recv(cs->conn);
			conn_xprt_want_send(cs->conn);
		}
	}

	 

	if (cs->flags & CS_FL_DATA_WR_ENA) {
		if (LIST_ISEMPTY(&h2s->list)) {
			if (LIST_ISEMPTY(&h2s->h2c->send_list) &&
			    !h2s->h2c->mbuf->o &&  
			    !(cs->conn->flags & CO_FL_SOCK_WR_SH))
				conn_xprt_want_send(cs->conn);
			LIST_ADDQ(&h2s->h2c->send_list, &h2s->list);
		}
	}
	else if (!LIST_ISEMPTY(&h2s->list)) {
		LIST_DEL(&h2s->list);
		LIST_INIT(&h2s->list);
		h2s->flags &= ~(H2_SF_BLK_MBUSY | H2_SF_BLK_MROOM | H2_SF_BLK_MFCTL);
	}

	 
	if (h2s->h2c->mbuf->o && !(cs->conn->flags & CO_FL_XPRT_WR_ENA))
		conn_xprt_want_send(cs->conn);
}
