static void h2_wake_some_streams(struct h2c *h2c, int last, uint32_t flags)
{
	struct eb32_node *node;
	struct h2s *h2s;

	if (h2c->st0 >= H2_CS_ERROR || h2c->conn->flags & CO_FL_ERROR)
		flags |= CS_FL_ERROR;

	if (conn_xprt_read0_pending(h2c->conn))
		flags |= CS_FL_EOS;

	node = eb32_lookup_ge(&h2c->streams_by_id, last + 1);
	while (node) {
		h2s = container_of(node, struct h2s, by_id);
		if (h2s->id <= last)
			break;
		node = eb32_next(node);

		if (!h2s->cs) {
			 
			h2s_destroy(h2s);
			continue;
		}

		h2s->cs->flags |= flags;
		 
		h2s->cs->data_cb->recv(h2s->cs);
		h2s->cs->data_cb->wake(h2s->cs);

		if (flags & CS_FL_ERROR && h2s->st < H2_SS_ERROR)
			h2s->st = H2_SS_ERROR;
		else if (flags & CS_FL_EOS && h2s->st == H2_SS_OPEN)
			h2s->st = H2_SS_HREM;
		else if (flags & CS_FL_EOS && h2s->st == H2_SS_HLOC)
			h2s_close(h2s);
	}
}