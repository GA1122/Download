int http_response_forward_body(struct session *s, struct channel *res, int an_bit)
{
	struct http_txn *txn = &s->txn;
	struct http_msg *msg = &s->txn.rsp;
	static struct buffer *tmpbuf = NULL;
	int compressing = 0;
	int ret;

	if (unlikely(msg->msg_state < HTTP_MSG_BODY))
		return 0;

	if ((res->flags & (CF_READ_ERROR|CF_READ_TIMEOUT|CF_WRITE_ERROR|CF_WRITE_TIMEOUT)) ||
	    ((res->flags & CF_SHUTW) && (res->to_forward || res->buf->o)) ||
	    !s->req->analysers) {
		 
		msg->msg_state = HTTP_MSG_ERROR;
		http_resync_states(s);
		return 1;
	}

	 
	channel_auto_close(res);

	if (msg->sov > 0) {
		 
		b_adv(res->buf, msg->sov);
		msg->next -= msg->sov;
		msg->sov = 0;

		 
		if (msg->msg_state < HTTP_MSG_CHUNK_SIZE) {
			if (msg->flags & HTTP_MSGF_TE_CHNK)
				msg->msg_state = HTTP_MSG_CHUNK_SIZE;
			else
				msg->msg_state = HTTP_MSG_DATA;
		}
	}

	if (res->to_forward) {
		 
		res->flags |= CF_WAKE_WRITE;
		goto missing_data;
	}

	if (unlikely(s->comp_algo != NULL) && msg->msg_state < HTTP_MSG_TRAILERS) {
		 
		if (unlikely(tmpbuf == NULL)) {
			 
			tmpbuf = pool_alloc2(pool2_buffer);
			if (tmpbuf == NULL)
				goto aborted_xfer;  
		}

		ret = http_compression_buffer_init(s, res->buf, tmpbuf);
		if (ret < 0) {
			res->flags |= CF_WAKE_WRITE;
			goto missing_data;  
		}
		compressing = 1;
	}

	while (1) {
		switch (msg->msg_state - HTTP_MSG_DATA) {
		case HTTP_MSG_DATA - HTTP_MSG_DATA:	 
			 
			if (unlikely(s->comp_algo)) {
				ret = http_compression_buffer_add_data(s, res->buf, tmpbuf);
				if (ret < 0)
					goto aborted_xfer;

				if (msg->chunk_len) {
					 
					if (res->buf->i > msg->next)
						res->flags |= CF_WAKE_WRITE;
					goto missing_data;
				}
			}
			else {
				if (msg->chunk_len > res->buf->i - msg->next) {
					 
					res->flags |= CF_WAKE_WRITE;
					goto missing_data;
				}
				msg->next += msg->chunk_len;
				msg->chunk_len = 0;
			}

			 
			if (msg->flags & HTTP_MSGF_TE_CHNK) {
				msg->msg_state = HTTP_MSG_CHUNK_CRLF;
			} else {
				msg->msg_state = HTTP_MSG_DONE;
				break;
			}
			 

		case HTTP_MSG_CHUNK_CRLF - HTTP_MSG_DATA:
			 

			ret = http_skip_chunk_crlf(msg);
			if (ret == 0)
				goto missing_data;
			else if (ret < 0) {
				if (msg->err_pos >= 0)
					http_capture_bad_message(&s->be->invalid_rep, s, msg, HTTP_MSG_CHUNK_CRLF, s->fe);
				goto return_bad_res;
			}
			 

		case HTTP_MSG_CHUNK_SIZE - HTTP_MSG_DATA:
			 

			ret = http_parse_chunk_size(msg);
			if (ret == 0)
				goto missing_data;
			else if (ret < 0) {
				if (msg->err_pos >= 0)
					http_capture_bad_message(&s->be->invalid_rep, s, msg, HTTP_MSG_CHUNK_SIZE, s->fe);
				goto return_bad_res;
			}
			 
			break;

		case HTTP_MSG_TRAILERS - HTTP_MSG_DATA:
			if (unlikely(compressing)) {
				 
				http_compression_buffer_end(s, &res->buf, &tmpbuf, 1);
				compressing = 0;
			}

			ret = http_forward_trailers(msg);
			if (ret == 0)
				goto missing_data;
			else if (ret < 0) {
				if (msg->err_pos >= 0)
					http_capture_bad_message(&s->be->invalid_rep, s, msg, HTTP_MSG_TRAILERS, s->fe);
				goto return_bad_res;
			}
			 

		default:
			 
			if (unlikely(compressing)) {
				 
				http_compression_buffer_end(s, &res->buf, &tmpbuf, 1);
				compressing = 0;
			}

			 
			b_adv(res->buf, msg->next);
			msg->next = 0;

			ret = msg->msg_state;
			 
			if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL ||
			    (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL)
				channel_dont_close(res);

			if (http_resync_states(s)) {
				 
				if (unlikely(msg->msg_state == HTTP_MSG_ERROR)) {
					if (res->flags & CF_SHUTW) {
						 
						goto aborted_xfer;
					}
					if (msg->err_pos >= 0)
						http_capture_bad_message(&s->be->invalid_rep, s, msg, ret, s->fe);
					goto return_bad_res;
				}
				return 1;
			}
			return 0;
		}
	}

 missing_data:
	 
	if (unlikely(compressing)) {
		http_compression_buffer_end(s, &res->buf, &tmpbuf, msg->msg_state >= HTTP_MSG_TRAILERS);
		compressing = 0;
	}

	if ((s->comp_algo == NULL || msg->msg_state >= HTTP_MSG_TRAILERS)) {
		b_adv(res->buf, msg->next);
		msg->next = 0;
		msg->chunk_len -= channel_forward(res, msg->chunk_len);
	}

	if (res->flags & CF_SHUTW)
		goto aborted_xfer;

	 
	if (res->flags & CF_SHUTR) {
		if ((s->req->flags & (CF_SHUTR|CF_SHUTW)) == (CF_SHUTR|CF_SHUTW))
			goto aborted_xfer;
		if (!(s->flags & SN_ERR_MASK))
			s->flags |= SN_ERR_SRVCL;
		s->be->be_counters.srv_aborts++;
		if (objt_server(s->target))
			objt_server(s->target)->counters.srv_aborts++;
		goto return_bad_res_stats_ok;
	}

	 
	if (!s->req->analysers)
		goto return_bad_res;

	 
	if ((msg->flags & HTTP_MSGF_TE_CHNK) || s->comp_algo ||
	    (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL ||
	    (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL)
		channel_dont_close(res);

	 
	if ((msg->flags & HTTP_MSGF_TE_CHNK) || s->comp_algo)
		res->flags |= CF_EXPECT_MORE;

	 
	return 0;

 return_bad_res:  
	s->be->be_counters.failed_resp++;
	if (objt_server(s->target))
		objt_server(s->target)->counters.failed_resp++;

 return_bad_res_stats_ok:
	if (unlikely(compressing)) {
		http_compression_buffer_end(s, &res->buf, &tmpbuf, msg->msg_state >= HTTP_MSG_TRAILERS);
		compressing = 0;
	}

	 
	if (s->comp_algo == NULL) {
		b_adv(res->buf, msg->next);
		msg->next = 0;
	}

	txn->rsp.msg_state = HTTP_MSG_ERROR;
	 
	stream_int_retnclose(res->cons, NULL);
	res->analysers = 0;
	s->req->analysers = 0;  
	if (objt_server(s->target))
		health_adjust(objt_server(s->target), HANA_STATUS_HTTP_HDRRSP);

	if (!(s->flags & SN_ERR_MASK))
		s->flags |= SN_ERR_PRXCOND;
	if (!(s->flags & SN_FINST_MASK))
		s->flags |= SN_FINST_D;
	return 0;

 aborted_xfer:
	if (unlikely(compressing)) {
		http_compression_buffer_end(s, &res->buf, &tmpbuf, msg->msg_state >= HTTP_MSG_TRAILERS);
		compressing = 0;
	}

	txn->rsp.msg_state = HTTP_MSG_ERROR;
	 
	stream_int_retnclose(res->cons, NULL);
	res->analysers = 0;
	s->req->analysers = 0;  

	s->fe->fe_counters.cli_aborts++;
	s->be->be_counters.cli_aborts++;
	if (objt_server(s->target))
		objt_server(s->target)->counters.cli_aborts++;

	if (!(s->flags & SN_ERR_MASK))
		s->flags |= SN_ERR_CLICL;
	if (!(s->flags & SN_FINST_MASK))
		s->flags |= SN_FINST_D;
	return 0;
}
