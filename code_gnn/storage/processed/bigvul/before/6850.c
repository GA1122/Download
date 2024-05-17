void http_reset_txn(struct stream *s)
{
	http_end_txn(s);
	http_init_txn(s);

	 
	s->current_rule_list = NULL;

	s->be = strm_fe(s);
	s->logs.logwait = strm_fe(s)->to_log;
	s->logs.level = 0;
	stream_del_srv_conn(s);
	s->target = NULL;
	 
	s->store_count = 0;
	s->uniq_id = global.req_count++;

	s->req.flags |= CF_READ_DONTWAIT;  

	 
	if (unlikely(s->res.buf->i))
		s->res.buf->i = 0;

	 
	buffer_realign(s->res.buf);

	s->req.rto = strm_fe(s)->timeout.client;
	s->req.wto = TICK_ETERNITY;

	s->res.rto = TICK_ETERNITY;
	s->res.wto = strm_fe(s)->timeout.client;

	s->req.rex = TICK_ETERNITY;
	s->req.wex = TICK_ETERNITY;
	s->req.analyse_exp = TICK_ETERNITY;
	s->res.rex = TICK_ETERNITY;
	s->res.wex = TICK_ETERNITY;
	s->res.analyse_exp = TICK_ETERNITY;
	s->si[1].hcto = TICK_ETERNITY;
}
