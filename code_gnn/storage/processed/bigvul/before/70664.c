nameserver_probe_callback(int result, char type, int count, int ttl, void *addresses, void *arg) {
	struct nameserver *const ns = (struct nameserver *) arg;
	(void) type;
	(void) count;
	(void) ttl;
	(void) addresses;

	if (result == DNS_ERR_CANCEL) {
		 
		return;
	}

	EVDNS_LOCK(ns->base);
	ns->probe_request = NULL;
	if (result == DNS_ERR_NONE || result == DNS_ERR_NOTEXIST) {
		 
		nameserver_up(ns);
	} else {
		nameserver_probe_failed(ns);
	}
	EVDNS_UNLOCK(ns->base);
}