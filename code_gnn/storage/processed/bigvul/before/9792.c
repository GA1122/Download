http_get_path(struct http_txn *txn)
{
	char *ptr, *end;

	ptr = txn->req.chn->buf->p + txn->req.sl.rq.u;
	end = ptr + txn->req.sl.rq.u_l;

	if (ptr >= end)
		return NULL;

	 

	if (*ptr == '*')
		return NULL;

	if (isalpha((unsigned char)*ptr)) {
		 
		ptr++;
		while (ptr < end &&
		       (isalnum((unsigned char)*ptr) || *ptr == '+' || *ptr == '-' || *ptr == '.'))
			ptr++;
		 
		if (ptr == end || *ptr++ != ':')
			return NULL;
		if (ptr == end || *ptr++ != '/')
			return NULL;
		if (ptr == end || *ptr++ != '/')
			return NULL;
	}
	 

	while (ptr < end && *ptr != '/')
		ptr++;

	if (ptr == end)
		return NULL;

	 
	return ptr;
}
