struct chunk *http_error_message(struct stream *s)
{
	const int msgnum = http_get_status_idx(s->txn->status);

	if (s->be->errmsg[msgnum].str)
		return &s->be->errmsg[msgnum];
	else if (strm_fe(s)->errmsg[msgnum].str)
		return &strm_fe(s)->errmsg[msgnum];
	else
		return &http_err_chunks[msgnum];
}
