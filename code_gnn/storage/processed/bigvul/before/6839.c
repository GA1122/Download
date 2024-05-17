void http_perform_server_redirect(struct stream *s, struct stream_interface *si)
{
	struct http_txn *txn;
	struct server *srv;
	char *path;
	int len, rewind;

	 
	trash.len = strlen(HTTP_302);
	memcpy(trash.str, HTTP_302, trash.len);

	srv = objt_server(s->target);

	 
	if (trash.len + srv->rdr_len > trash.size)
		return;

	 
	if (srv->rdr_len != 1 || *srv->rdr_pfx != '/') {
		memcpy(trash.str + trash.len, srv->rdr_pfx, srv->rdr_len);
		trash.len += srv->rdr_len;
	}

	 
	txn = s->txn;
	b_rew(s->req.buf, rewind = http_hdr_rewind(&txn->req));

	path = http_get_path(txn);
	len = buffer_count(s->req.buf, path, b_ptr(s->req.buf, txn->req.sl.rq.u + txn->req.sl.rq.u_l));

	b_adv(s->req.buf, rewind);

	if (!path)
		return;

	if (trash.len + len > trash.size - 4)  
		return;

	memcpy(trash.str + trash.len, path, len);
	trash.len += len;

	if (unlikely(txn->flags & TX_USE_PX_CONN)) {
		memcpy(trash.str + trash.len, "\r\nProxy-Connection: close\r\n\r\n", 29);
		trash.len += 29;
	} else {
		memcpy(trash.str + trash.len, "\r\nConnection: close\r\n\r\n", 23);
		trash.len += 23;
	}

	 
	si_shutr(si);
	si_shutw(si);
	si->err_type = SI_ET_NONE;
	si->state    = SI_ST_CLO;

	 
	txn->status = 302;
	http_server_error(s, si, SF_ERR_LOCAL, SF_FINST_C, &trash);

	 
	srv_inc_sess_ctr(srv);
	srv_set_sess_last(srv);
}
