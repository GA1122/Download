static int bgp_attr_aggregator(struct bgp_attr_parser_args *args)
{
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	const bgp_size_t length = args->length;

	int wantedlen = 6;

	 
	if (CHECK_FLAG(peer->cap, PEER_CAP_AS4_RCV))
		wantedlen = 8;

	if (length != wantedlen) {
		flog_err(EC_BGP_ATTR_LEN,
			 "AGGREGATOR attribute length isn't %u [%u]", wantedlen,
			 length);
		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
					  args->total);
	}

	if (CHECK_FLAG(peer->cap, PEER_CAP_AS4_RCV))
		attr->aggregator_as = stream_getl(peer->curr);
	else
		attr->aggregator_as = stream_getw(peer->curr);
	attr->aggregator_addr.s_addr = stream_get_ipv4(peer->curr);

	 
	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_AGGREGATOR);

	return BGP_ATTR_PARSE_PROCEED;
}