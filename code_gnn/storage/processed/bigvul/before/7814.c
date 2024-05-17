static int h2s_frt_make_resp_data(struct h2s *h2s, struct buffer *buf)
{
	struct h2c *h2c = h2s->h2c;
	struct h1m *h1m = &h2s->res;
	struct chunk outbuf;
	int ret = 0;
	int total = 0;
	int es_now = 0;
	int size = 0;
	char *blk1, *blk2;
	int len1, len2;

	if (h2c_mux_busy(h2c, h2s)) {
		h2s->flags |= H2_SF_BLK_MBUSY;
		goto end;
	}

	if (!h2_get_buf(h2c, &h2c->mbuf)) {
		h2c->flags |= H2_CF_MUX_MALLOC;
		h2s->flags |= H2_SF_BLK_MROOM;
		goto end;
	}

 new_frame:
	if (!buf->o)
		goto end;

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
		goto end;
	}

	 
	memcpy(outbuf.str, "\x00\x00\x00\x00\x00", 5);
	write_n32(outbuf.str + 5, h2s->id);  
	outbuf.len = 9;

	switch (h1m->flags & (H1_MF_CLEN|H1_MF_CHNK)) {
	case 0:            
		size = buf->o;
		h1m->curr_len = size;
		break;
	case H1_MF_CLEN:   
		size = buf->o;
		if ((long long)size > h1m->curr_len)
			size = h1m->curr_len;
		break;
	default:           
		if (h1m->state == HTTP_MSG_CHUNK_CRLF) {
			ret = h1_skip_chunk_crlf(buf, -buf->o, 0);
			if (!ret)
				goto end;

			if (ret < 0) {
				 
				h1m->err_pos = ret;
				h2s_error(h2s, H2_ERR_INTERNAL_ERROR);
				goto end;
			}
			bo_del(buf, ret);
			total += ret;
			h1m->state = HTTP_MSG_CHUNK_SIZE;
		}

		if (h1m->state == HTTP_MSG_CHUNK_SIZE) {
			unsigned int chunk;

			ret = h1_parse_chunk_size(buf, -buf->o, 0, &chunk);
			if (!ret)
				goto end;

			if (ret < 0) {
				 
				h1m->err_pos = ret;
				h2s_error(h2s, H2_ERR_INTERNAL_ERROR);
				goto end;
			}

			size = chunk;
			h1m->curr_len = chunk;
			h1m->body_len += chunk;
			bo_del(buf, ret);
			total += ret;
			h1m->state = size ? HTTP_MSG_DATA : HTTP_MSG_TRAILERS;
			if (!size)
				goto send_empty;
		}

		 
		size = h1m->curr_len;
		break;
	}

	 

	if (size > buf->o)
		size = buf->o;

	if (size > h2s->mws)
		size = h2s->mws;

	if (size <= 0) {
		h2s->flags |= H2_SF_BLK_SFCTL;
		goto end;
	}

	if (h2c->mfs && size > h2c->mfs)
		size = h2c->mfs;

	if (size + 9 > outbuf.size) {
		 
		if (buffer_space_wraps(h2c->mbuf))
			goto realign_again;
		size = outbuf.size - 9;
	}

	if (size <= 0) {
		h2c->flags |= H2_CF_MUX_MFULL;
		h2s->flags |= H2_SF_BLK_MROOM;
		goto end;
	}

	if (size > h2c->mws)
		size = h2c->mws;

	if (size <= 0) {
		h2s->flags |= H2_SF_BLK_MFCTL;
		goto end;
	}

	 
	blk1 = blk2 = NULL;  
	ret = bo_getblk_nc(buf, &blk1, &len1, &blk2, &len2);
	if (ret == 1)
		len2 = 0;

	if (!ret || len1 + len2 < size) {
		 
		h2s_error(h2s, H2_ERR_INTERNAL_ERROR);
		goto end;
	}

	 
	if (len1 + len2 > size) {
		int sub = len1 + len2 - size;

		if (len2 > sub)
			len2 -= sub;
		else {
			sub -= len2;
			len2 = 0;
			len1 -= sub;
		}
	}

	 
	memcpy(outbuf.str + 9, blk1, len1);
	if (len2)
		memcpy(outbuf.str + 9 + len1, blk2, len2);

 send_empty:
	 
	 
	if (((h1m->flags & H1_MF_CLEN) && !(h1m->curr_len - size)) ||
	    !h1m->curr_len || h1m->state >= HTTP_MSG_DONE)
		es_now = 1;

	 
	h2_set_frame_size(outbuf.str, size);

	if (es_now)
		outbuf.str[4] |= H2_F_DATA_END_STREAM;

	 
	h2c->mbuf->o += size + 9;
	h2c->mbuf->p = b_ptr(h2c->mbuf, size + 9);

	 
	if (size > 0) {
		bo_del(buf, size);
		total += size;
		h1m->curr_len -= size;
		h2s->mws -= size;
		h2c->mws -= size;

		if (size && !h1m->curr_len && (h1m->flags & H1_MF_CHNK)) {
			h1m->state = HTTP_MSG_CHUNK_CRLF;
			goto new_frame;
		}
	}

	if (es_now) {
		if (h2s->st == H2_SS_OPEN)
			h2s->st = H2_SS_HLOC;
		else
			h2s_close(h2s);

		if (!(h1m->flags & H1_MF_CHNK)) {
			bo_del(buf, buf->o);

			h1m->state = HTTP_MSG_DONE;
		}

		h2s->flags |= H2_SF_ES_SENT;
	}

 end:
	trace("[%d] sent simple H2 DATA response (sid=%d) = %d bytes out (%d in, st=%s, ep=%u, es=%s, h2cws=%d h2sws=%d) buf->o=%d", h2c->st0, h2s->id, size+9, total, h1_msg_state_str(h1m->state), h1m->err_pos, h1_msg_state_str(h1m->err_state), h2c->mws, h2s->mws, buf->o);
	return total;
}