static int h2_frt_decode_headers(struct h2s *h2s, struct buffer *buf, int count)
{
	struct h2c *h2c = h2s->h2c;
	const uint8_t *hdrs = (uint8_t *)h2c->dbuf->p;
	struct chunk *tmp = get_trash_chunk();
	struct http_hdr list[MAX_HTTP_HDR * 2];
	struct chunk *copy = NULL;
	int flen = h2c->dfl;
	int outlen = 0;
	int wrap;
	int try;

	if (!h2c->dfl) {
		h2s_error(h2s, H2_ERR_PROTOCOL_ERROR);  
		h2c->st0 = H2_CS_FRAME_E;
		return 0;
	}

	if (h2c->dbuf->i < h2c->dfl && h2c->dbuf->i < h2c->dbuf->size)
		return 0;  

	 
	wrap = h2c->dbuf->data + h2c->dbuf->size - h2c->dbuf->p;
	if (wrap < h2c->dfl) {
		copy = alloc_trash_chunk();
		if (!copy) {
			h2c_error(h2c, H2_ERR_INTERNAL_ERROR);
			goto fail;
		}
		memcpy(copy->str, h2c->dbuf->p, wrap);
		memcpy(copy->str + wrap, h2c->dbuf->data, h2c->dfl - wrap);
		hdrs = (uint8_t *)copy->str;
	}

	 
	if (h2c->dff & H2_F_HEADERS_PADDED) {
		h2c->dpl = *hdrs;
		if (h2c->dpl >= flen) {
			 
			h2c_error(h2c, H2_ERR_PROTOCOL_ERROR);
			return 0;
		}
		flen -= h2c->dpl + 1;
		hdrs += 1;  
	}

	 
	if (h2c->dff & H2_F_HEADERS_PRIORITY) {
		if (read_n32(hdrs) == h2s->id) {
			 
			h2c_error(h2c, H2_ERR_PROTOCOL_ERROR);
			return 0; 
		}

		hdrs += 5;  
		flen -= 5;
	}

	 
	if (!(h2c->dff & H2_F_HEADERS_END_HEADERS)) {
		h2c_error(h2c, H2_ERR_INTERNAL_ERROR);
		goto fail;
	}

	 
	if (unlikely(buf->o)) {
		 
		goto fail;
	}

	if (unlikely(buffer_space_wraps(buf))) {
		 
		buffer_slow_realign(buf);
	}

	 
	try = buf->data + buf->size - (buf->p + buf->i);

	 
	if (try <= 0) {
		try = buf->p - (buf->data + buf->o);
		if (try <= 0)
			goto fail;
	}
	if (try > count)
		try = count;

	outlen = hpack_decode_frame(h2c->ddht, hdrs, flen, list,
	                            sizeof(list)/sizeof(list[0]), tmp);
	if (outlen < 0) {
		h2c_error(h2c, H2_ERR_COMPRESSION_ERROR);
		goto fail;
	}

	 
	outlen = h2_make_h1_request(list, bi_end(buf), try);

	if (outlen < 0) {
		h2c_error(h2c, H2_ERR_COMPRESSION_ERROR);
		goto fail;
	}

	 
	bi_del(h2c->dbuf, h2c->dfl);
	h2c->st0 = H2_CS_FRAME_H;
	buf->i += outlen;

	 
	if (h2c->dff & H2_F_HEADERS_END_STREAM) {
		h2s->cs->flags |= CS_FL_EOS;
		h2s->flags |= H2_SF_ES_RCVD;
	}

 leave:
	free_trash_chunk(copy);
	return outlen;
 fail:
	outlen = 0;
	goto leave;
}
