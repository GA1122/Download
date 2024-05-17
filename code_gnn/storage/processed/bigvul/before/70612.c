evdns_getaddrinfo_gotresolve(int result, char type, int count,
    int ttl, void *addresses, void *arg)
{
	int i;
	struct getaddrinfo_subrequest *req = arg;
	struct getaddrinfo_subrequest *other_req;
	struct evdns_getaddrinfo_request *data;

	struct evutil_addrinfo *res;

	struct sockaddr_in sin;
	struct sockaddr_in6 sin6;
	struct sockaddr *sa;
	int socklen, addrlen;
	void *addrp;
	int err;
	int user_canceled;

	EVUTIL_ASSERT(req->type == DNS_IPv4_A || req->type == DNS_IPv6_AAAA);
	if (req->type == DNS_IPv4_A) {
		data = EVUTIL_UPCAST(req, struct evdns_getaddrinfo_request, ipv4_request);
		other_req = &data->ipv6_request;
	} else {
		data = EVUTIL_UPCAST(req, struct evdns_getaddrinfo_request, ipv6_request);
		other_req = &data->ipv4_request;
	}

	 
	if (result != DNS_ERR_SHUTDOWN) {
		EVDNS_LOCK(data->evdns_base);
		if (evdns_result_is_answer(result)) {
			if (req->type == DNS_IPv4_A)
				++data->evdns_base->getaddrinfo_ipv4_answered;
			else
				++data->evdns_base->getaddrinfo_ipv6_answered;
		}
		user_canceled = data->user_canceled;
		if (other_req->r == NULL)
			data->request_done = 1;
		EVDNS_UNLOCK(data->evdns_base);
	} else {
		data->evdns_base = NULL;
		user_canceled = data->user_canceled;
	}

	req->r = NULL;

	if (result == DNS_ERR_CANCEL && ! user_canceled) {
		 
		if (other_req->r == NULL)
			free_getaddrinfo_request(data);
		return;
	}

	if (data->user_cb == NULL) {
		 
		free_getaddrinfo_request(data);
		return;
	}

	if (result == DNS_ERR_NONE) {
		if (count == 0)
			err = EVUTIL_EAI_NODATA;
		else
			err = 0;
	} else {
		err = evdns_err_to_getaddrinfo_err(result);
	}

	if (err) {
		 
		if (other_req->r) {
			 
			 
			if (result != DNS_ERR_SHUTDOWN) {
				evdns_getaddrinfo_set_timeout(data->evdns_base, data);
			}
			data->pending_error = err;
			return;
		}

		if (user_canceled) {
			data->user_cb(EVUTIL_EAI_CANCEL, NULL, data->user_data);
		} else if (data->pending_result) {
			 
			add_cname_to_reply(data, data->pending_result);
			data->user_cb(0, data->pending_result, data->user_data);
			data->pending_result = NULL;
		} else {
			if (data->pending_error)
				err = getaddrinfo_merge_err(err,
				    data->pending_error);
			data->user_cb(err, NULL, data->user_data);
		}
		free_getaddrinfo_request(data);
		return;
	} else if (user_canceled) {
		if (other_req->r) {
			 
			return;
		}
		data->user_cb(EVUTIL_EAI_CANCEL, NULL, data->user_data);
		free_getaddrinfo_request(data);
		return;
	}

	 
	EVUTIL_ASSERT(type == DNS_IPv4_A || type == DNS_IPv6_AAAA);

	if (type == DNS_IPv4_A) {
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(data->port);

		sa = (struct sockaddr *)&sin;
		socklen = sizeof(sin);
		addrlen = 4;
		addrp = &sin.sin_addr.s_addr;
	} else {
		memset(&sin6, 0, sizeof(sin6));
		sin6.sin6_family = AF_INET6;
		sin6.sin6_port = htons(data->port);

		sa = (struct sockaddr *)&sin6;
		socklen = sizeof(sin6);
		addrlen = 16;
		addrp = &sin6.sin6_addr.s6_addr;
	}

	res = NULL;
	for (i=0; i < count; ++i) {
		struct evutil_addrinfo *ai;
		memcpy(addrp, ((char*)addresses)+i*addrlen, addrlen);
		ai = evutil_new_addrinfo_(sa, socklen, &data->hints);
		if (!ai) {
			if (other_req->r) {
				evdns_cancel_request(NULL, other_req->r);
			}
			data->user_cb(EVUTIL_EAI_MEMORY, NULL, data->user_data);
			if (res)
				evutil_freeaddrinfo(res);

			if (other_req->r == NULL)
				free_getaddrinfo_request(data);
			return;
		}
		res = evutil_addrinfo_append_(res, ai);
	}

	if (other_req->r) {
		 
		 
		evdns_getaddrinfo_set_timeout(data->evdns_base, data);
		data->pending_result = res;
		return;
	} else {
		 
		if (data->pending_result) {
			if (req->type == DNS_IPv4_A)
				res = evutil_addrinfo_append_(res,
				    data->pending_result);
			else
				res = evutil_addrinfo_append_(
				    data->pending_result, res);
			data->pending_result = NULL;
		}

		 
		add_cname_to_reply(data, res);
		data->user_cb(0, res, data->user_data);

		 
		free_getaddrinfo_request(data);
	}
}
