request_reissue(struct request *req) {
	const struct nameserver *const last_ns = req->ns;
	ASSERT_LOCKED(req->base);
	ASSERT_VALID_REQUEST(req);
	 
	 
	 
	request_swap_ns(req, nameserver_pick(req->base));
	if (req->ns == last_ns) {
		 
		 
		 
		return 1;
	}

	req->reissue_count++;
	req->tx_count = 0;
	req->transmit_me = 1;

	return 0;
}
