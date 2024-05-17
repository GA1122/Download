int http_resync_states(struct session *s)
{
	struct http_txn *txn = &s->txn;
	int old_req_state = txn->req.msg_state;
	int old_res_state = txn->rsp.msg_state;

	http_sync_req_state(s);
	while (1) {
		if (!http_sync_res_state(s))
			break;
		if (!http_sync_req_state(s))
			break;
	}

	 

	if (txn->req.msg_state == HTTP_MSG_TUNNEL ||
	    txn->rsp.msg_state == HTTP_MSG_TUNNEL ||
	    (txn->req.msg_state == HTTP_MSG_CLOSED &&
	     txn->rsp.msg_state == HTTP_MSG_CLOSED)) {
		s->req->analysers = 0;
		channel_auto_close(s->req);
		channel_auto_read(s->req);
		s->rep->analysers = 0;
		channel_auto_close(s->rep);
		channel_auto_read(s->rep);
	}
	else if ((txn->req.msg_state >= HTTP_MSG_DONE &&
		  (txn->rsp.msg_state == HTTP_MSG_CLOSED || (s->rep->flags & CF_SHUTW))) ||
		 txn->rsp.msg_state == HTTP_MSG_ERROR ||
		 txn->req.msg_state == HTTP_MSG_ERROR) {
		s->rep->analysers = 0;
		channel_auto_close(s->rep);
		channel_auto_read(s->rep);
		s->req->analysers = 0;
		channel_abort(s->req);
		channel_auto_close(s->req);
		channel_auto_read(s->req);
		bi_erase(s->req);
	}
	else if ((txn->req.msg_state == HTTP_MSG_DONE ||
		  txn->req.msg_state == HTTP_MSG_CLOSED) &&
		 txn->rsp.msg_state == HTTP_MSG_DONE &&
		 ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL ||
		  (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL)) {
		 
		http_end_txn_clean_session(s);
	}

	return txn->req.msg_state != old_req_state ||
		txn->rsp.msg_state != old_res_state;
}
