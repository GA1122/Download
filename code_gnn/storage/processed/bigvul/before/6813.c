void http_adjust_conn_mode(struct stream *s, struct http_txn *txn, struct http_msg *msg)
{
	struct proxy *fe = strm_fe(s);
	int tmp = TX_CON_WANT_KAL;

	if (!((fe->options2|s->be->options2) & PR_O2_FAKE_KA)) {
		if ((fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_TUN ||
		    (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_TUN)
			tmp = TX_CON_WANT_TUN;

		if ((fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL ||
		    (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL)
			tmp = TX_CON_WANT_TUN;
	}

	if ((fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_SCL ||
	    (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_SCL) {
		 
		if ((fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL ||
		    (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL)
			tmp = TX_CON_WANT_CLO;
		else
			tmp = TX_CON_WANT_SCL;
	}

	if ((fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_FCL ||
	    (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_FCL)
		tmp = TX_CON_WANT_CLO;

	if ((txn->flags & TX_CON_WANT_MSK) < tmp)
		txn->flags = (txn->flags & ~TX_CON_WANT_MSK) | tmp;

	if (!(txn->flags & TX_HDR_CONN_PRS) &&
	    (txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_TUN) {
		 
		int to_del = 0;
		if ((msg->flags & HTTP_MSGF_VER_11) ||
		    ((txn->flags & TX_CON_WANT_MSK) >= TX_CON_WANT_SCL &&
		     !((fe->options2|s->be->options2) & PR_O2_FAKE_KA)))
			to_del |= 2;  
		if (!(msg->flags & HTTP_MSGF_VER_11))
			to_del |= 1;  
		http_parse_connection_header(txn, msg, to_del);
	}

	 
	if (((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL ||
	     (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL) &&
	    ((txn->flags & TX_HDR_CONN_CLO) ||                          
	     (!(msg->flags & HTTP_MSGF_VER_11) && !(txn->flags & TX_HDR_CONN_KAL)) ||  
	     !(msg->flags & HTTP_MSGF_XFER_LEN) ||                      
	     fe->state == PR_STSTOPPED))                             
		txn->flags = (txn->flags & ~TX_CON_WANT_MSK) | TX_CON_WANT_CLO;
}
