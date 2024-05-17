 void http_end_txn_clean_session(struct session *s)
 {
 	int prev_status = s->txn.status;
 
 	 
 
 	 
 	if (((s->txn.flags & TX_CON_WANT_MSK) != TX_CON_WANT_KAL) ||
 	    !si_conn_ready(s->req->cons)) {
 		s->req->cons->flags |= SI_FL_NOLINGER | SI_FL_NOHALF;
 		si_shutr(s->req->cons);
 		si_shutw(s->req->cons);
 	}
 
 	if (s->flags & SN_BE_ASSIGNED) {
 		s->be->beconn--;
 		if (unlikely(s->srv_conn))
 			sess_change_server(s, NULL);
 	}
 
 	s->logs.t_close = tv_ms_elapsed(&s->logs.tv_accept, &now);
 	session_process_counters(s);
 
 	if (s->txn.status) {
 		int n;
 
 		n = s->txn.status / 100;
 		if (n < 1 || n > 5)
 			n = 0;
 
 		if (s->fe->mode == PR_MODE_HTTP) {
 			s->fe->fe_counters.p.http.rsp[n]++;
 			if (s->comp_algo && (s->flags & SN_COMP_READY))
 				s->fe->fe_counters.p.http.comp_rsp++;
 		}
 		if ((s->flags & SN_BE_ASSIGNED) &&
 		    (s->be->mode == PR_MODE_HTTP)) {
 			s->be->be_counters.p.http.rsp[n]++;
 			s->be->be_counters.p.http.cum_req++;
 			if (s->comp_algo && (s->flags & SN_COMP_READY))
 				s->be->be_counters.p.http.comp_rsp++;
 		}
 	}
 
 	 
 	s->logs.bytes_in  -= s->req->buf->i;
 	s->logs.bytes_out -= s->rep->buf->i;
 
 	 
 	if (!LIST_ISEMPTY(&s->fe->logformat) && s->logs.logwait &&
 	    !(s->flags & SN_MONITOR) &&
 	    (!(s->fe->options & PR_O_NULLNOLOG) || s->req->total)) {
 		s->do_log(s);
 	}
 
 	 
 	session_stop_content_counters(s);
 	session_update_time_stats(s);
 
 	s->logs.accept_date = date;  
 	s->logs.tv_accept = now;   
 	tv_zero(&s->logs.tv_request);
 	s->logs.t_queue = -1;
 	s->logs.t_connect = -1;
 	s->logs.t_data = -1;
 	s->logs.t_close = 0;
 	s->logs.prx_queue_size = 0;   
 	s->logs.srv_queue_size = 0;  
 
 	s->logs.bytes_in = s->req->total = s->req->buf->i;
 	s->logs.bytes_out = s->rep->total = s->rep->buf->i;
 
 	if (s->pend_pos)
 		pendconn_free(s->pend_pos);
 
 	if (objt_server(s->target)) {
 		if (s->flags & SN_CURR_SESS) {
 			s->flags &= ~SN_CURR_SESS;
 			objt_server(s->target)->cur_sess--;
 		}
 		if (may_dequeue_tasks(objt_server(s->target), s->be))
 			process_srv_queue(objt_server(s->target));
 	}
 
 	s->target = NULL;
 
 	 
 	if (((s->txn.flags & TX_CON_WANT_MSK) != TX_CON_WANT_KAL) ||
 	    !si_conn_ready(s->req->cons)) {
 		si_release_endpoint(s->req->cons);
 	}
 
 	s->req->cons->state     = s->req->cons->prev_state = SI_ST_INI;
 	s->req->cons->err_type  = SI_ET_NONE;
         s->req->cons->conn_retries = 0;   
         s->req->cons->exp       = TICK_ETERNITY;
         s->req->cons->flags    &= SI_FL_DONT_WAKE;  
       s->req->flags &= ~(CF_SHUTW|CF_SHUTW_NOW|CF_AUTO_CONNECT|CF_WRITE_ERROR|CF_STREAMER|CF_STREAMER_FAST|CF_NEVER_WAIT|CF_WAKE_CONNECT);
       s->rep->flags &= ~(CF_SHUTR|CF_SHUTR_NOW|CF_READ_ATTACHED|CF_READ_ERROR|CF_READ_NOEXP|CF_STREAMER|CF_STREAMER_FAST|CF_WRITE_PARTIAL|CF_NEVER_WAIT);
//        s->req->flags &= ~(CF_SHUTW|CF_SHUTW_NOW|CF_AUTO_CONNECT|CF_WRITE_ERROR|CF_STREAMER|CF_STREAMER_FAST|CF_NEVER_WAIT|CF_WAKE_CONNECT|CF_WROTE_DATA);
//        s->rep->flags &= ~(CF_SHUTR|CF_SHUTR_NOW|CF_READ_ATTACHED|CF_READ_ERROR|CF_READ_NOEXP|CF_STREAMER|CF_STREAMER_FAST|CF_WRITE_PARTIAL|CF_NEVER_WAIT|CF_WROTE_DATA);
         s->flags &= ~(SN_DIRECT|SN_ASSIGNED|SN_ADDR_SET|SN_BE_ASSIGNED|SN_FORCE_PRST|SN_IGNORE_PRST);
         s->flags &= ~(SN_CURR_SESS|SN_REDIRECTABLE|SN_SRV_REUSED);
  
 	s->txn.meth = 0;
 	http_reset_txn(s);
 	s->txn.flags |= TX_NOT_FIRST | TX_WAIT_NEXT_RQ;
 
 	if (prev_status == 401 || prev_status == 407) {
 		 
 		s->txn.flags |= TX_PREFER_LAST;
 	}
 
 	if (s->fe->options2 & PR_O2_INDEPSTR)
 		s->req->cons->flags |= SI_FL_INDEP_STR;
 
 	if (s->fe->options2 & PR_O2_NODELAY) {
 		s->req->flags |= CF_NEVER_WAIT;
 		s->rep->flags |= CF_NEVER_WAIT;
 	}
 
 	 
 	if (s->req->buf->i) {
 		if (s->rep->buf->o &&
 		    !buffer_full(s->rep->buf, global.tune.maxrewrite) &&
 		    bi_end(s->rep->buf) <= s->rep->buf->data + s->rep->buf->size - global.tune.maxrewrite)
 			s->rep->flags |= CF_EXPECT_MORE;
 	}
 
 	 
 	channel_auto_read(s->req);
 	channel_auto_close(s->req);
 	channel_auto_read(s->rep);
 	channel_auto_close(s->rep);
 
 	 
 	si_idle_conn(s->req->cons);
 
 	s->req->analysers = s->listener->analysers;
 	s->rep->analysers = 0;
 }