free_getaddrinfo_request(struct evdns_getaddrinfo_request *data)
{
	 
	if (data->pending_result)
		evutil_freeaddrinfo(data->pending_result);
	if (data->cname_result)
		mm_free(data->cname_result);
	event_del(&data->timeout);
	mm_free(data);
	return;
}
