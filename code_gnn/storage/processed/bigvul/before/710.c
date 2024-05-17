dns_run_resolution(struct dns_resolution *resolution)
{
	struct dns_resolvers  *resolvers = resolution->resolvers;
	int query_id, i;

	 
	if (!resolution->hostname_dn)
		return 0;

	 
	if (resolution->step != RSLV_STEP_NONE)
		return 0;

	 
	for (i = 0; i < 100; ++i) {
		query_id = dns_rnd16();
		if (!eb32_lookup(&resolvers->query_ids, query_id))
			break;
		query_id = -1;
	}
	if (query_id == -1) {
		send_log(NULL, LOG_NOTICE,
			 "could not generate a query id for %s, in resolvers %s.\n",
			 resolution->hostname_dn, resolvers->id);
		return -1;
	}

	 
	resolution->query_id     = query_id;
	resolution->qid.key      = query_id;
	resolution->step         = RSLV_STEP_RUNNING;
	resolution->query_type   = resolution->prefered_query_type;
	resolution->try          = resolvers->resolve_retries;
	eb32_insert(&resolvers->query_ids, &resolution->qid);

	 
	resolution->try -= 1;
	dns_send_query(resolution);
	return 1;
}
