 static void h2_process_demux(struct h2c *h2c)
 {
 	struct h2s *h2s;
 
 	if (h2c->st0 >= H2_CS_ERROR)
 		return;
 
 	if (unlikely(h2c->st0 < H2_CS_FRAME_H)) {
 		if (h2c->st0 == H2_CS_PREFACE) {
 			if (unlikely(h2c_frt_recv_preface(h2c) <= 0)) {
 				 
 				if (h2c->st0 == H2_CS_ERROR)
 					h2c->st0 = H2_CS_ERROR2;
 				goto fail;
 			}
 
 			h2c->max_id = 0;
 			h2c->st0 = H2_CS_SETTINGS1;
 		}
 
 		if (h2c->st0 == H2_CS_SETTINGS1) {
 			struct h2_fh hdr;
 
 			 
 			if (!h2_get_frame_hdr(h2c->dbuf, &hdr)) {
 				 
 				if (h2c->st0 == H2_CS_ERROR)
 					h2c->st0 = H2_CS_ERROR2;
 				goto fail;
 			}
 
 			if (hdr.sid || hdr.ft != H2_FT_SETTINGS || hdr.ff & H2_F_SETTINGS_ACK) {
 				 
 				h2c_error(h2c, H2_ERR_PROTOCOL_ERROR);
 				h2c->st0 = H2_CS_ERROR2;
                                 goto fail;
                         }
  
                       if ((int)hdr.len < 0 || (int)hdr.len > h2c->mfs) {
//                        if ((int)hdr.len < 0 || (int)hdr.len > global.tune.bufsize) {
                                  
                                 h2c_error(h2c, H2_ERR_FRAME_SIZE_ERROR);
                                 h2c->st0 = H2_CS_ERROR2;
 				goto fail;
 			}
 
 			 
 			h2c->dfl = hdr.len;
 			h2c->dsi = hdr.sid;
 			h2c->dft = hdr.ft;
 			h2c->dff = hdr.ff;
 			h2c->dpl = 0;
 			h2c->st0 = H2_CS_FRAME_P;
 		}
 	}
 
 	 
 	while (h2c->dbuf->i) {
 		int ret = 0;
 
 		if (h2c->st0 >= H2_CS_ERROR)
 			break;
 
 		if (h2c->st0 == H2_CS_FRAME_H) {
 			struct h2_fh hdr;
 
                         if (!h2_peek_frame_hdr(h2c->dbuf, &hdr))
                                 break;
  
                       if ((int)hdr.len < 0 || (int)hdr.len > h2c->mfs) {
//                        if ((int)hdr.len < 0 || (int)hdr.len > global.tune.bufsize) {
                                 h2c_error(h2c, H2_ERR_FRAME_SIZE_ERROR);
                                 h2c->st0 = H2_CS_ERROR;
                                 break;
 			}
 
 			h2c->dfl = hdr.len;
 			h2c->dsi = hdr.sid;
 			h2c->dft = hdr.ft;
 			h2c->dff = hdr.ff;
 			h2c->dpl = 0;
 			h2c->st0 = H2_CS_FRAME_P;
 			h2_skip_frame_hdr(h2c->dbuf);
 		}
 
 		 
 		h2s = h2c_st_by_id(h2c, h2c->dsi);
 
 		if (h2c->st0 == H2_CS_FRAME_E)
 			goto strm_err;
 
 		if (h2s->st == H2_SS_IDLE &&
 		    h2c->dft != H2_FT_HEADERS && h2c->dft != H2_FT_PRIORITY) {
 			 
 			h2c_error(h2c, H2_ERR_PROTOCOL_ERROR);
 			h2c->st0 = H2_CS_ERROR;
 			break;
 		}
 
 		if (h2s->st == H2_SS_HREM && h2c->dft != H2_FT_WINDOW_UPDATE &&
 		    h2c->dft != H2_FT_RST_STREAM && h2c->dft != H2_FT_PRIORITY) {
 			 
 			h2s_error(h2s, H2_ERR_STREAM_CLOSED);
 			h2c->st0 = H2_CS_FRAME_E;
 			goto strm_err;
 		}
 
 		 
 		if (h2s->st == H2_SS_CLOSED && h2c->dsi) {
 			if (h2c->dft == H2_FT_HEADERS || h2c->dft == H2_FT_PUSH_PROMISE) {
 				 
 				h2c_error(h2c, H2_ERR_STREAM_CLOSED);
 				goto strm_err;
 			}
 
 			if (h2s->flags & H2_SF_RST_RCVD) {
 				 
 				h2s_error(h2s, H2_ERR_STREAM_CLOSED);
 				h2c->st0 = H2_CS_FRAME_E;
 				goto strm_err;
 			}
 
 			 
 			if (!(h2s->flags & H2_SF_RST_SENT)) {
 				 
 				if (h2c->dft != H2_FT_RST_STREAM &&
 				    h2c->dft != H2_FT_PRIORITY &&
 				    h2c->dft != H2_FT_WINDOW_UPDATE) {
 					h2c_error(h2c, H2_ERR_STREAM_CLOSED);
 					goto strm_err;
 				}
 			}
 		}
 
 #if 0
 
 		 
 		if (unlikely(h2c->last_sid >= 0) && h2c->dsi > h2c->last_sid) {
 			ret = MIN(h2c->dbuf->i, h2c->dfl);
 			bi_del(h2c->dbuf, ret);
 			h2c->dfl -= ret;
 			ret = h2c->dfl == 0;
 			goto strm_err;
 		}
 #endif
 
 		switch (h2c->dft) {
 		case H2_FT_SETTINGS:
 			if (h2c->st0 == H2_CS_FRAME_P)
 				ret = h2c_handle_settings(h2c);
 
 			if (h2c->st0 == H2_CS_FRAME_A)
 				ret = h2c_ack_settings(h2c);
 			break;
 
 		case H2_FT_PING:
 			if (h2c->st0 == H2_CS_FRAME_P)
 				ret = h2c_handle_ping(h2c);
 
 			if (h2c->st0 == H2_CS_FRAME_A)
 				ret = h2c_ack_ping(h2c);
 			break;
 
 		case H2_FT_WINDOW_UPDATE:
 			if (h2c->st0 == H2_CS_FRAME_P)
 				ret = h2c_handle_window_update(h2c, h2s);
 			break;
 
 		case H2_FT_CONTINUATION:
 			 
 			if (h2c->st0 == H2_CS_FRAME_P) {
 				h2s_error(h2s, H2_ERR_INTERNAL_ERROR);
 				h2c->st0 = H2_CS_FRAME_E;
 			}
 			break;
 
 		case H2_FT_HEADERS:
 			if (h2c->st0 == H2_CS_FRAME_P)
 				ret = h2c_frt_handle_headers(h2c, h2s);
 			break;
 
 		case H2_FT_DATA:
 			if (h2c->st0 == H2_CS_FRAME_P)
 				ret = h2c_frt_handle_data(h2c, h2s);
 
 			if (h2c->st0 == H2_CS_FRAME_A)
 				ret = h2c_send_strm_wu(h2c);
 			break;
 
 		case H2_FT_PRIORITY:
 			if (h2c->st0 == H2_CS_FRAME_P)
 				ret = h2c_handle_priority(h2c);
 			break;
 
 		case H2_FT_RST_STREAM:
 			if (h2c->st0 == H2_CS_FRAME_P)
 				ret = h2c_handle_rst_stream(h2c, h2s);
 			break;
 
 		case H2_FT_GOAWAY:
 			if (h2c->st0 == H2_CS_FRAME_P)
 				ret = h2c_handle_goaway(h2c);
 			break;
 
 		case H2_FT_PUSH_PROMISE:
 			 
 			h2c_error(h2c, H2_ERR_PROTOCOL_ERROR);
 			break;
 
 			 
 		default:
 			 
 			ret = MIN(h2c->dbuf->i, h2c->dfl);
 			bi_del(h2c->dbuf, ret);
 			h2c->dfl -= ret;
 			ret = h2c->dfl == 0;
 		}
 
 	strm_err:
 		 
 		if (h2s->st == H2_SS_ERROR)
 			h2c->st0 = H2_CS_FRAME_E;
 
 		if (h2c->st0 == H2_CS_FRAME_E)
 			ret = h2c_send_rst_stream(h2c, h2s);
 
 		 
 		if (ret <= 0)
 			break;
 
 		if (h2c->st0 != H2_CS_FRAME_H) {
 			bi_del(h2c->dbuf, h2c->dfl);
 			h2c->st0 = H2_CS_FRAME_H;
 		}
 	}
 
 	if (h2c->rcvd_c > 0 &&
 	    !(h2c->flags & (H2_CF_MUX_MFULL | H2_CF_DEM_MBUSY | H2_CF_DEM_MROOM)))
 		h2c_send_conn_wu(h2c);
 
  fail:
 	 
 	return;
 }