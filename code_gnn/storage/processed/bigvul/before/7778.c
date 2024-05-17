static void h2_send(struct connection *conn)
{
	struct h2c *h2c = conn->mux_ctx;
	int done;

	if (conn->flags & CO_FL_ERROR)
		return;

	if (conn->flags & (CO_FL_HANDSHAKE|CO_FL_WAIT_L4_CONN|CO_FL_WAIT_L6_CONN)) {
		 
		return;
	}

	 

	done = 0;
	while (!done) {
		unsigned int flags = 0;

		 
		while (((h2c->flags & (H2_CF_MUX_MFULL|H2_CF_MUX_MALLOC)) == 0) && !done)
			done = h2_process_mux(h2c);

		if (conn->flags & CO_FL_ERROR)
			break;

		if (h2c->flags & (H2_CF_MUX_MFULL | H2_CF_DEM_MBUSY | H2_CF_DEM_MROOM))
			flags |= CO_SFL_MSG_MORE;

		if (h2c->mbuf->o && conn->xprt->snd_buf(conn, h2c->mbuf, flags) <= 0)
			break;

		 
		h2c->flags &= ~(H2_CF_MUX_MFULL | H2_CF_DEM_MROOM);
	}

	if (conn->flags & CO_FL_SOCK_WR_SH) {
		 
		h2c->mbuf->o = 0;
	}
}
