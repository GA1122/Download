static int bgp_attr_as4_path(struct bgp_attr_parser_args *args,
			     struct aspath **as4_path)
{
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	const bgp_size_t length = args->length;

	*as4_path = aspath_parse(peer->curr, length, 1);

	 
	if (!*as4_path) {
		flog_err(EC_BGP_ATTR_MAL_AS_PATH,
			 "Malformed AS4 path from %s, length is %d", peer->host,
			 length);
		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_MAL_AS_PATH,
					  0);
	}

	 
	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_AS4_PATH);

	return BGP_ATTR_PARSE_PROCEED;
}