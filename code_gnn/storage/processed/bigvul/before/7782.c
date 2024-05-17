static void h2_shutr(struct conn_stream *cs, enum cs_shr_mode mode)
{
	struct h2s *h2s = cs->ctx;

	if (!mode)
		return;

	if (h2s->st == H2_SS_HLOC || h2s->st == H2_SS_ERROR || h2s->st == H2_SS_CLOSED)
		return;

	 
	if (!(h2s->flags & H2_SF_RST_SENT) &&
	    h2s_send_rst_stream(h2s->h2c, h2s) <= 0)
		goto add_to_list;

	if (!(h2s->flags & H2_SF_OUTGOING_DATA) &&
	    !(h2s->h2c->flags & (H2_CF_GOAWAY_SENT|H2_CF_GOAWAY_FAILED)) &&
	    h2c_send_goaway_error(h2s->h2c, h2s) <= 0)
		goto add_to_list;

	if (h2s->h2c->mbuf->o && !(cs->conn->flags & CO_FL_XPRT_WR_ENA))
		conn_xprt_want_send(cs->conn);

	h2s_close(h2s);

 add_to_list:
	if (LIST_ISEMPTY(&h2s->list)) {
		if (h2s->flags & H2_SF_BLK_MFCTL)
			LIST_ADDQ(&h2s->h2c->fctl_list, &h2s->list);
		else if (h2s->flags & (H2_SF_BLK_MBUSY|H2_SF_BLK_MROOM))
			LIST_ADDQ(&h2s->h2c->send_list, &h2s->list);
	}
}
