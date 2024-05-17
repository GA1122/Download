void http_resync_states(struct stream *s)
{
	struct http_txn *txn = s->txn;
#ifdef DEBUG_FULL
	int old_req_state = txn->req.msg_state;
	int old_res_state = txn->rsp.msg_state;
#endif

	http_sync_req_state(s);
	while (1) {
		if (!http_sync_res_state(s))
			break;
		if (!http_sync_req_state(s))
			break;
	}

	DPRINTF(stderr,"[%u] %s: stream=%p old=%s,%s cur=%s,%s "
		"req->analysers=0x%08x res->analysers=0x%08x\n",
		now_ms, __FUNCTION__, s,
		h1_msg_state_str(old_req_state), h1_msg_state_str(old_res_state),
		h1_msg_state_str(txn->req.msg_state), h1_msg_state_str(txn->rsp.msg_state),
		s->req.analysers, s->res.analysers);


	 
	if (txn->req.msg_state == HTTP_MSG_CLOSED &&
	    txn->rsp.msg_state == HTTP_MSG_CLOSED) {
		s->req.analysers &= AN_REQ_FLT_END;
		channel_auto_close(&s->req);
		channel_auto_read(&s->req);
		s->res.analysers &= AN_RES_FLT_END;
		channel_auto_close(&s->res);
		channel_auto_read(&s->res);
	}
	else if (txn->rsp.msg_state == HTTP_MSG_CLOSED ||
		 txn->rsp.msg_state == HTTP_MSG_ERROR  ||
		 txn->req.msg_state == HTTP_MSG_ERROR) {
		s->res.analysers &= AN_RES_FLT_END;
		channel_auto_close(&s->res);
		channel_auto_read(&s->res);
		s->req.analysers &= AN_REQ_FLT_END;
		channel_abort(&s->req);
		channel_auto_close(&s->req);
		channel_auto_read(&s->req);
		channel_truncate(&s->req);
	}
	else if (txn->req.msg_state == HTTP_MSG_TUNNEL ||
		 txn->rsp.msg_state == HTTP_MSG_TUNNEL) {
		if (txn->req.msg_state == HTTP_MSG_TUNNEL) {
			s->req.analysers &= AN_REQ_FLT_END;
			if (HAS_REQ_DATA_FILTERS(s))
				s->req.analysers |= AN_REQ_FLT_XFER_DATA;
		}
		if (txn->rsp.msg_state == HTTP_MSG_TUNNEL) {
			s->res.analysers &= AN_RES_FLT_END;
			if (HAS_RSP_DATA_FILTERS(s))
				s->res.analysers |= AN_RES_FLT_XFER_DATA;
		}
		channel_auto_close(&s->req);
		channel_auto_read(&s->req);
		channel_auto_close(&s->res);
		channel_auto_read(&s->res);
	}
	else if ((txn->req.msg_state == HTTP_MSG_DONE ||
		  txn->req.msg_state == HTTP_MSG_CLOSED) &&
		 txn->rsp.msg_state == HTTP_MSG_DONE &&
		 ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL ||
		  (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL)) {
		 
		if (s->req.buf->o)
			s->req.flags |= CF_WAKE_WRITE;
		else if (s->res.buf->o)
			s->res.flags |= CF_WAKE_WRITE;
		else {
			s->req.analysers = AN_REQ_FLT_END;
			s->res.analysers = AN_RES_FLT_END;
			txn->flags |= TX_WAIT_CLEANUP;
		}
	}
}
