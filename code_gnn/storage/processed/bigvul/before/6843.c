int http_process_tarpit(struct stream *s, struct channel *req, int an_bit)
{
	struct http_txn *txn = s->txn;

	 
	channel_dont_connect(req);
	if ((req->flags & (CF_SHUTR|CF_READ_ERROR)) == 0 &&
	    !tick_is_expired(req->analyse_exp, now_ms))
		return 0;

	 
	s->logs.t_queue = tv_ms_elapsed(&s->logs.tv_accept, &now);

	if (!(req->flags & CF_READ_ERROR))
		http_reply_and_close(s, txn->status, http_error_message(s));

	req->analysers &= AN_REQ_FLT_END;
	req->analyse_exp = TICK_ETERNITY;

	if (!(s->flags & SF_ERR_MASK))
		s->flags |= SF_ERR_PRXCOND;
	if (!(s->flags & SF_FINST_MASK))
		s->flags |= SF_FINST_T;
	return 0;
}
