static int h2s_frt_make_resp_headers(struct h2s *h2s, struct buffer *buf)
{
	struct http_hdr list[MAX_HTTP_HDR];
	struct h2c *h2c = h2s->h2c;
	struct h1m *h1m = &h2s->res;
	struct chunk outbuf;
	int es_now = 0;
	int ret = 0;
	int hdr;

	if (h2c_mux_busy(h2c, h2s)) {
		h2s->flags |= H2_SF_BLK_MBUSY;
		return 0;
	}

	if (!h2_get_buf(h2c, &h2c->mbuf)) {
		h2c->flags |= H2_CF_MUX_MALLOC;
		h2s->flags |= H2_SF_BLK_MROOM;
		return 0;
	}

	 
	ret = h1_headers_to_hdr_list(bo_ptr(buf), bo_ptr(buf) + buf->o,
	                             list, sizeof(list)/sizeof(list[0]), h1m);
	if (ret <= 0) {
		 
		h2s_error(h2s, H2_ERR_INTERNAL_ERROR);
		ret = 0;
		goto end;
	}

	chunk_reset(&outbuf);

	while (1) {
		outbuf.str  = bo_end(h2c->mbuf);
		outbuf.size = bo_contig_space(h2c->mbuf);
		outbuf.len = 0;

		if (outbuf.size >= 9 || !buffer_space_wraps(h2c->mbuf))
			break;
	realign_again:
		buffer_slow_realign(h2c->mbuf);
	}

	if (outbuf.size < 9) {
		h2c->flags |= H2_CF_MUX_MFULL;
		h2s->flags |= H2_SF_BLK_MROOM;
		ret = 0;
		goto end;
	}

	 
	memcpy(outbuf.str, "\x00\x00\x00\x01\x04", 5);
	write_n32(outbuf.str + 5, h2s->id);  
	outbuf.len = 9;

	 
	if (outbuf.len < outbuf.size && h1m->status == 200)
		outbuf.str[outbuf.len++] = 0x88;  
	else if (outbuf.len < outbuf.size && h1m->status == 304)
		outbuf.str[outbuf.len++] = 0x8b;  
	else if (unlikely(list[0].v.len != 3)) {
		 
		h2s_error(h2s, H2_ERR_INTERNAL_ERROR);
		ret = 0;
		goto end;
	}
	else if (unlikely(outbuf.len + 2 + 3 <= outbuf.size)) {
		 
		outbuf.str[outbuf.len++] = 0x48;  
		outbuf.str[outbuf.len++] = 0x03;  
		outbuf.str[outbuf.len++] = list[0].v.ptr[0];
		outbuf.str[outbuf.len++] = list[0].v.ptr[1];
		outbuf.str[outbuf.len++] = list[0].v.ptr[2];
	}
	else {
		if (buffer_space_wraps(h2c->mbuf))
			goto realign_again;

		h2c->flags |= H2_CF_MUX_MFULL;
		h2s->flags |= H2_SF_BLK_MROOM;
		ret = 0;
		goto end;
	}

	 
	for (hdr = 1; hdr < sizeof(list)/sizeof(list[0]); hdr++) {
		 
		if (isteq(list[hdr].n, ist("connection")) ||
		    isteq(list[hdr].n, ist("proxy-connection")) ||
		    isteq(list[hdr].n, ist("keep-alive")) ||
		    isteq(list[hdr].n, ist("upgrade")) ||
		    isteq(list[hdr].n, ist("transfer-encoding")))
			continue;

		if (isteq(list[hdr].n, ist("")))
			break;  

		if (!hpack_encode_header(&outbuf, list[hdr].n, list[hdr].v)) {
			 
			if (buffer_space_wraps(h2c->mbuf))
				goto realign_again;

			h2c->flags |= H2_CF_MUX_MFULL;
			h2s->flags |= H2_SF_BLK_MROOM;
			ret = 0;
			goto end;
		}
	}

	 
	if (((h1m->flags & H1_MF_CLEN) && !h1m->body_len) || h2s->cs->flags & CS_FL_SHW)
		es_now = 1;

	 
	h2_set_frame_size(outbuf.str, outbuf.len - 9);

	if (es_now)
		outbuf.str[4] |= H2_F_HEADERS_END_STREAM;

	 
	bo_del(buf, ret);

	 
	h2c->mbuf->o += outbuf.len;
	h2c->mbuf->p = b_ptr(h2c->mbuf, outbuf.len);
	h2s->flags |= H2_SF_HEADERS_SENT;

	 
	if (es_now) {
		bo_del(buf, buf->o);

		h1m->state = HTTP_MSG_DONE;
		h2s->flags |= H2_SF_ES_SENT;
		if (h2s->st == H2_SS_OPEN)
			h2s->st = H2_SS_HLOC;
		else
			h2s_close(h2s);
	}
	else if (h1m->status >= 100 && h1m->status < 200) {
		 
		h1m->state = HTTP_MSG_RPBEFORE;
		h1m->status = 0;
		h1m->flags = 0;
		goto end;
	}
	else
		h1m->state = (h1m->flags & H1_MF_CHNK) ? HTTP_MSG_CHUNK_SIZE : HTTP_MSG_BODY;

 end:
	return ret;
}
