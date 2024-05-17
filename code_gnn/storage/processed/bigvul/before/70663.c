nameserver_failed(struct nameserver *const ns, const char *msg) {
	struct request *req, *started_at;
	struct evdns_base *base = ns->base;
	int i;
	char addrbuf[128];

	ASSERT_LOCKED(base);
	 
	 
	if (!ns->state) return;

	log(EVDNS_LOG_MSG, "Nameserver %s has failed: %s",
	    evutil_format_sockaddr_port_(
		    (struct sockaddr *)&ns->address,
		    addrbuf, sizeof(addrbuf)),
	    msg);

	base->global_good_nameservers--;
	EVUTIL_ASSERT(base->global_good_nameservers >= 0);
	if (base->global_good_nameservers == 0) {
		log(EVDNS_LOG_MSG, "All nameservers have failed");
	}

	ns->state = 0;
	ns->failed_times = 1;

	if (evtimer_add(&ns->timeout_event,
		&base->global_nameserver_probe_initial_timeout) < 0) {
		log(EVDNS_LOG_WARN,
		    "Error from libevent when adding timer event for %s",
		    evutil_format_sockaddr_port_(
			    (struct sockaddr *)&ns->address,
			    addrbuf, sizeof(addrbuf)));
		 
	}

	 
	 
	 

	 
	 
	if (!base->global_good_nameservers) return;

	for (i = 0; i < base->n_req_heads; ++i) {
		req = started_at = base->req_heads[i];
		if (req) {
			do {
				if (req->tx_count == 0 && req->ns == ns) {
					 
					 
					request_swap_ns(req, nameserver_pick(base));
				}
				req = req->next;
			} while (req != started_at);
		}
	}
}
