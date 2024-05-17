http_msg_forward_body(struct stream *s, struct http_msg *msg)
{
	struct channel *chn = msg->chn;
	int ret;

	 

	if (msg->msg_state == HTTP_MSG_ENDING)
		goto ending;

	 
	if (!(msg->flags & HTTP_MSGF_XFER_LEN)) {
		unsigned long long len = (chn->buf->i - msg->next);
		msg->chunk_len += len;
		msg->body_len  += len;
	}
	ret = FLT_STRM_DATA_CB(s, chn, flt_http_data(s, msg),
			         MIN(msg->chunk_len, chn->buf->i - msg->next),
			         goto error);
	msg->next     += ret;
	msg->chunk_len -= ret;
	if (msg->chunk_len) {
		 
		if (chn->buf->i > msg->next)
			chn->flags |= CF_WAKE_WRITE;
		goto missing_data_or_waiting;
	}

	 
	if (!(msg->flags & HTTP_MSGF_XFER_LEN)) {
		 
		if (!(chn->flags & CF_SHUTR) && HAS_DATA_FILTERS(s, chn))
			goto missing_data_or_waiting;
		msg->msg_state = HTTP_MSG_TUNNEL;
		goto ending;
	}

	msg->msg_state = HTTP_MSG_ENDING;

  ending:
	 
	ret = FLT_STRM_DATA_CB(s, chn, flt_http_forward_data(s, msg, msg->next),
			         msg->next,
			         goto error);
	b_adv(chn->buf, ret);
	msg->next -= ret;
	if (unlikely(!(chn->flags & CF_WROTE_DATA) || msg->sov > 0))
		msg->sov -= ret;
	if (msg->next)
		goto waiting;

	FLT_STRM_DATA_CB(s, chn, flt_http_end(s, msg),
			   1,
			   goto error,
			   goto waiting);
	if (msg->msg_state == HTTP_MSG_ENDING)
		msg->msg_state = HTTP_MSG_DONE;
	return 1;

  missing_data_or_waiting:
	 
	ret = FLT_STRM_DATA_CB(s, chn, flt_http_forward_data(s, msg, msg->next),
			         msg->next,
			         goto error);
	b_adv(chn->buf, ret);
	msg->next -= ret;
	if (!(chn->flags & CF_WROTE_DATA) || msg->sov > 0)
		msg->sov -= ret;
	if (!HAS_DATA_FILTERS(s, chn))
		msg->chunk_len -= channel_forward(chn, msg->chunk_len);
  waiting:
	return 0;
  error:
	return -1;
}
