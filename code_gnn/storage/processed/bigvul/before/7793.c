static int h2c_frt_handle_headers(struct h2c *h2c, struct h2s *h2s)
{
	int error;

	if (!h2c->dfl) {
		error = H2_ERR_PROTOCOL_ERROR;  
		goto strm_err;
	}

	if (!h2c->dbuf->size)
		return 0;  

	if (h2c->dbuf->i < h2c->dfl && h2c->dbuf->i < h2c->dbuf->size)
		return 0;  

	 
	if (h2s->st != H2_SS_IDLE) {
		 
		error = H2_ERR_PROTOCOL_ERROR;
		goto conn_err;
	}
	else if (h2c->dsi <= h2c->max_id || !(h2c->dsi & 1)) {
		 
		error = H2_ERR_PROTOCOL_ERROR;
		goto conn_err;
	}

	h2s = h2c_stream_new(h2c, h2c->dsi);
	if (!h2s) {
		error = H2_ERR_INTERNAL_ERROR;
		goto conn_err;
	}

	h2s->st = H2_SS_OPEN;
	if (h2c->dff & H2_F_HEADERS_END_STREAM) {
		h2s->st = H2_SS_HREM;
		h2s->flags |= H2_SF_ES_RCVD;
	}

	 
	h2s->cs->data_cb->recv(h2s->cs);

	if (h2s->cs->data_cb->wake(h2s->cs) < 0) {
		 
		error = H2_ERR_INTERNAL_ERROR;
		goto conn_err;
	}

	if (h2c->st0 >= H2_CS_ERROR)
		return 0;

	if (h2s->st >= H2_SS_ERROR) {
		 
		h2c->st0 = H2_CS_FRAME_E;
	}
	else {
		 
		if (h2s->id > h2c->max_id)
			h2c->max_id = h2s->id;
	}

	return 1;

 conn_err:
	h2c_error(h2c, error);
	return 0;

 strm_err:
	if (h2s) {
		h2s_error(h2s, error);
		h2c->st0 = H2_CS_FRAME_E;
	}
	else
		h2c_error(h2c, error);
	return 0;
}
