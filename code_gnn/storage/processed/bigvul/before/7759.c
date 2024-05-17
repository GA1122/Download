static void h2_detach(struct conn_stream *cs)
{
	struct h2s *h2s = cs->ctx;
	struct h2c *h2c;

	cs->ctx = NULL;
	if (!h2s)
		return;

	h2c = h2s->h2c;
	h2s->cs = NULL;

	 
	if (!(cs->conn->flags & CO_FL_ERROR) &&
	    (h2s->flags & (H2_SF_BLK_MBUSY | H2_SF_BLK_MROOM | H2_SF_BLK_MFCTL)))
		return;

	if ((h2c->flags & H2_CF_DEM_BLOCK_ANY && h2s->id == h2c->dsi) ||
	    (h2c->flags & H2_CF_MUX_BLOCK_ANY && h2s->id == h2c->msi)) {
		 
		h2c->flags &= ~H2_CF_DEM_BLOCK_ANY;
		h2c->flags &= ~H2_CF_MUX_BLOCK_ANY;
		conn_xprt_want_recv(cs->conn);
		conn_xprt_want_send(cs->conn);
	}

	h2s_destroy(h2s);

	 
	if (eb_is_empty(&h2c->streams_by_id) &&      
	    ((h2c->conn->flags & CO_FL_ERROR) ||     
	     (h2c->flags & H2_CF_GOAWAY_FAILED) ||
	     (!h2c->mbuf->o &&   
	      (conn_xprt_read0_pending(h2c->conn) ||
	       (h2c->last_sid >= 0 && h2c->max_id >= h2c->last_sid))))) {
		 
		h2_release(h2c->conn);
	}
	else if (h2c->task) {
		if (eb_is_empty(&h2c->streams_by_id) || h2c->mbuf->o) {
			h2c->task->expire = tick_add(now_ms, h2c->last_sid < 0 ? h2c->timeout : h2c->shut_timeout);
			task_queue(h2c->task);
		}
		else
			h2c->task->expire = TICK_ETERNITY;
	}
}
