int http_process_tarpit(struct session *s, struct channel *req, int an_bit)
{
	struct http_txn *txn = &s->txn;

	 
	channel_dont_connect(req);
	if ((req->flags & (CF_SHUTR|CF_READ_ERROR)) == 0 &&
	    !tick_is_expired(req->analyse_exp, now_ms))
		return 0;

	 
	s->logs.t_queue = tv_ms_elapsed(&s->logs.tv_accept, &now);

	txn->status = 500;
	if (!(req->flags & CF_READ_ERROR))
		stream_int_retnclose(req->prod, http_error_message(s, HTTP_ERR_500));

	req->analysers = 0;
	req->analyse_exp = TICK_ETERNITY;

	if (!(s->flags & SN_ERR_MASK))
		s->flags |= SN_ERR_PRXCOND;
	if (!(s->flags & SN_FINST_MASK))
		s->flags |= SN_FINST_T;
	return 0;
}