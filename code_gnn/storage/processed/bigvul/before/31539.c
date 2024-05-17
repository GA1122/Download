rad_init_send_request(struct rad_handle *h, int *fd, struct timeval *tv)
{
	int srv;

	 
	if (h->fd == -1) {
		struct sockaddr_in sin;

		if ((h->fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
#ifdef PHP_WIN32
			generr(h, "Cannot create socket: %d", WSAGetLastError());
#else
			generr(h, "Cannot create socket: %s", strerror(errno));
#endif
			return -1;
		}
		memset(&sin, 0, sizeof sin);
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = INADDR_ANY;
		sin.sin_port = htons(0);
		if (bind(h->fd, (const struct sockaddr *)&sin,
		    sizeof sin) == -1) {
#ifdef PHP_WIN32
			generr(h, "bind: %d", WSAGetLastError());
#else
			generr(h, "bind: %s", strerror(errno));
#endif
			close(h->fd);
			h->fd = -1;
			return -1;
		}
	}

	if (h->request[POS_CODE] == RAD_ACCOUNTING_REQUEST) {
		 
		if (h->pass_pos || h->chap_pass) {
			generr(h, "User or Chap Password in accounting request");
			return -1;
		}
	} else {
		 
		if (h->pass_pos == 0 && !h->chap_pass) {
			generr(h, "No User or Chap Password attributes given");
			return -1;
		}
		if (h->pass_pos != 0 && h->chap_pass) {
			generr(h, "Both User and Chap Password attributes given");
			return -1;
		}
	}

	 
	h->request[POS_LENGTH] = h->req_len >> 8;
	h->request[POS_LENGTH+1] = h->req_len;

	 
	h->total_tries = 0;
	for (srv = 0;  srv < h->num_servers;  srv++) {
		h->total_tries += h->servers[srv].max_tries;
		h->servers[srv].num_tries = 0;
	}
	if (h->total_tries == 0) {
		generr(h, "No RADIUS servers specified");
		return -1;
	}

	h->try = h->srv = 0;

	return rad_continue_send_request(h, 0, fd, tv);
}