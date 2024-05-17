bgp_attr_large_community(struct bgp_attr_parser_args *args)
{
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	const bgp_size_t length = args->length;

	 
	if (length == 0) {
		attr->lcommunity = NULL;
		 
		return BGP_ATTR_PARSE_PROCEED;
	}

	attr->lcommunity =
		lcommunity_parse((uint8_t *)stream_pnt(peer->curr), length);
	 
	stream_forward_getp(peer->curr, length);

	if (!attr->lcommunity)
		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_OPT_ATTR_ERR,
					  args->total);

	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_LARGE_COMMUNITIES);

	return BGP_ATTR_PARSE_PROCEED;
}
