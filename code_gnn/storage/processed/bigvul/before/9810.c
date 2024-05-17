void http_reset_txn(struct session *s)
{
	http_end_txn(s);
	http_init_txn(s);

	s->be = s->fe;
	s->logs.logwait = s->fe->to_log;
	s->logs.level = 0;
	session_del_srv_conn(s);
	s->target = NULL;
	 
	s->store_count = 0;
	s->uniq_id = global.req_count++;

	s->pend_pos = NULL;

	s->req->flags |= CF_READ_DONTWAIT;  

	 
	if (unlikely(s->rep->buf->i))
		s->rep->buf->i = 0;

	s->req->rto = s->fe->timeout.client;
	s->req->wto = TICK_ETERNITY;

	s->rep->rto = TICK_ETERNITY;
	s->rep->wto = s->fe->timeout.client;

	s->req->rex = TICK_ETERNITY;
	s->req->wex = TICK_ETERNITY;
	s->req->analyse_exp = TICK_ETERNITY;
	s->rep->rex = TICK_ETERNITY;
	s->rep->wex = TICK_ETERNITY;
	s->rep->analyse_exp = TICK_ETERNITY;
}