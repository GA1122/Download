void http_end_txn_clean_session(struct stream *s)
{
	int prev_status = s->txn->status;
	struct proxy *fe = strm_fe(s);
	struct proxy *be = s->be;
	struct conn_stream *cs;
	struct connection *srv_conn;
	struct server *srv;
	unsigned int prev_flags = s->txn->flags;

	 
	 
	cs = objt_cs(s->si[1].end);
	srv_conn = cs_conn(cs);

	 
	if (((s->txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_KAL) ||
	    !si_conn_ready(&s->si[1])) {
		s->si[1].flags |= SI_FL_NOLINGER | SI_FL_NOHALF;
		si_shutr(&s->si[1]);
		si_shutw(&s->si[1]);
	}

	if (s->flags & SF_BE_ASSIGNED) {
		HA_ATOMIC_SUB(&be->beconn, 1);
		if (unlikely(s->srv_conn))
			sess_change_server(s, NULL);
	}

	s->logs.t_close = tv_ms_elapsed(&s->logs.tv_accept, &now);
	stream_process_counters(s);

	if (s->txn->status) {
		int n;

		n = s->txn->status / 100;
		if (n < 1 || n > 5)
			n = 0;

		if (fe->mode == PR_MODE_HTTP) {
			HA_ATOMIC_ADD(&fe->fe_counters.p.http.rsp[n], 1);
		}
		if ((s->flags & SF_BE_ASSIGNED) &&
		    (be->mode == PR_MODE_HTTP)) {
			HA_ATOMIC_ADD(&be->be_counters.p.http.rsp[n], 1);
			HA_ATOMIC_ADD(&be->be_counters.p.http.cum_req, 1);
		}
	}

	 
	s->logs.bytes_in  -= s->req.buf->i;
	s->logs.bytes_out -= s->res.buf->i;

	 
	if (!LIST_ISEMPTY(&fe->logformat) && s->logs.logwait &&
	    !(s->flags & SF_MONITOR) &&
	    (!(fe->options & PR_O_NULLNOLOG) || s->req.total)) {
		s->do_log(s);
	}

	 
	stream_stop_content_counters(s);
	stream_update_time_stats(s);

	s->logs.accept_date = date;  
	s->logs.tv_accept = now;   
	s->logs.t_handshake = 0;  
	s->logs.t_idle = -1;
	tv_zero(&s->logs.tv_request);
	s->logs.t_queue = -1;
	s->logs.t_connect = -1;
	s->logs.t_data = -1;
	s->logs.t_close = 0;
	s->logs.prx_queue_size = 0;   
	s->logs.srv_queue_size = 0;  

	s->logs.bytes_in = s->req.total = s->req.buf->i;
	s->logs.bytes_out = s->res.total = s->res.buf->i;

	if (s->pend_pos)
		pendconn_free(s->pend_pos);

	if (objt_server(s->target)) {
		if (s->flags & SF_CURR_SESS) {
			s->flags &= ~SF_CURR_SESS;
			HA_ATOMIC_SUB(&objt_server(s->target)->cur_sess, 1);
		}
		if (may_dequeue_tasks(objt_server(s->target), be))
			process_srv_queue(objt_server(s->target));
	}

	s->target = NULL;

	 
	if (((s->txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_KAL) ||
	    !si_conn_ready(&s->si[1])) {
		si_release_endpoint(&s->si[1]);
		srv_conn = NULL;
	}

	s->si[1].state     = s->si[1].prev_state = SI_ST_INI;
	s->si[1].err_type  = SI_ET_NONE;
	s->si[1].conn_retries = 0;   
	s->si[1].exp       = TICK_ETERNITY;
	s->si[1].flags    &= SI_FL_ISBACK | SI_FL_DONT_WAKE;  
	s->req.flags &= ~(CF_SHUTW|CF_SHUTW_NOW|CF_AUTO_CONNECT|CF_WRITE_ERROR|CF_STREAMER|CF_STREAMER_FAST|CF_NEVER_WAIT|CF_WAKE_CONNECT|CF_WROTE_DATA);
	s->res.flags &= ~(CF_SHUTR|CF_SHUTR_NOW|CF_READ_ATTACHED|CF_READ_ERROR|CF_READ_NOEXP|CF_STREAMER|CF_STREAMER_FAST|CF_WRITE_PARTIAL|CF_NEVER_WAIT|CF_WROTE_DATA|CF_WRITE_EVENT);
	s->flags &= ~(SF_DIRECT|SF_ASSIGNED|SF_ADDR_SET|SF_BE_ASSIGNED|SF_FORCE_PRST|SF_IGNORE_PRST);
	s->flags &= ~(SF_CURR_SESS|SF_REDIRECTABLE|SF_SRV_REUSED);
	s->flags &= ~(SF_ERR_MASK|SF_FINST_MASK|SF_REDISP);

	s->txn->meth = 0;
	http_reset_txn(s);
	s->txn->flags |= TX_NOT_FIRST | TX_WAIT_NEXT_RQ;

	if (prev_status == 401 || prev_status == 407) {
		 
		s->txn->flags |= TX_PREFER_LAST;
		if (srv_conn)
			srv_conn->flags |= CO_FL_PRIVATE;
	}

	 
	if (srv_conn && (be->options & PR_O_REUSE_MASK) == PR_O_REUSE_NEVR)
		srv_conn->flags |= CO_FL_PRIVATE;

	if (fe->options2 & PR_O2_INDEPSTR)
		s->si[1].flags |= SI_FL_INDEP_STR;

	if (fe->options2 & PR_O2_NODELAY) {
		s->req.flags |= CF_NEVER_WAIT;
		s->res.flags |= CF_NEVER_WAIT;
	}

	 
	channel_auto_read(&s->req);
	channel_auto_close(&s->req);
	channel_auto_read(&s->res);

	 
	if (srv_conn && LIST_ISEMPTY(&srv_conn->list)) {
		srv = objt_server(srv_conn->target);
		if (!srv)
			si_idle_cs(&s->si[1], NULL);
		else if (srv_conn->flags & CO_FL_PRIVATE)
			si_idle_cs(&s->si[1], (srv->priv_conns ? &srv->priv_conns[tid] : NULL));
		else if (prev_flags & TX_NOT_FIRST)
			 
			si_idle_cs(&s->si[1], (srv->safe_conns ? &srv->safe_conns[tid] : NULL));
		else
			si_idle_cs(&s->si[1], (srv->idle_conns ? &srv->idle_conns[tid] : NULL));
	}
	s->req.analysers = strm_li(s) ? strm_li(s)->analysers : 0;
	s->res.analysers = 0;
}
