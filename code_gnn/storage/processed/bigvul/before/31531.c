rad_continue_send_request(struct rad_handle *h, int selected, int *fd,
                          struct timeval *tv)
{
	int n;

	if (selected) {
		struct sockaddr_in from;
		int fromlen;

		fromlen = sizeof from;
		h->resp_len = recvfrom(h->fd, h->response,
		    MSGSIZE, MSG_WAITALL, (struct sockaddr *)&from, &fromlen);
		if (h->resp_len == -1) {
#ifdef PHP_WIN32
			generr(h, "recfrom: %d", WSAGetLastError());
#else
			generr(h, "recvfrom: %s", strerror(errno));
#endif
			return -1;
		}
		if (is_valid_response(h, h->srv, &from)) {
			h->resp_len = h->response[POS_LENGTH] << 8 |
			    h->response[POS_LENGTH+1];
			h->resp_pos = POS_ATTRS;
			return h->response[POS_CODE];
		}
	}

	if (h->try == h->total_tries) {
		generr(h, "No valid RADIUS responses received");
		return -1;
	}

	 
	while (h->servers[h->srv].num_tries >= h->servers[h->srv].max_tries)
		if (++h->srv >= h->num_servers)
			h->srv = 0;

	if (h->request[POS_CODE] == RAD_ACCOUNTING_REQUEST)
		 
		insert_request_authenticator(h, h->srv);
	else
		 
		if (h->pass_pos != 0)
			insert_scrambled_password(h, h->srv);

	 
	n = sendto(h->fd, h->request, h->req_len, 0,
	    (const struct sockaddr *)&h->servers[h->srv].addr,
	    sizeof h->servers[h->srv].addr);
	if (n != h->req_len) {
		if (n == -1)
#ifdef PHP_WIN32
			generr(h, "sendto: %d", WSAGetLastError());
#else
			generr(h, "sendto: %s", strerror(errno));
#endif
		else
			generr(h, "sendto: short write");
		return -1;
	}

	h->try++;
	h->servers[h->srv].num_tries++;
	tv->tv_sec = h->servers[h->srv].timeout;
	tv->tv_usec = 0;
	*fd = h->fd;

	return 0;
}