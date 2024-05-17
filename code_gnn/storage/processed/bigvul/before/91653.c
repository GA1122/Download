static bgp_attr_parse_ret_t bgp_attr_unknown(struct bgp_attr_parser_args *args)
{
	bgp_size_t total = args->total;
	struct transit *transit;
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	uint8_t *const startp = args->startp;
	const uint8_t type = args->type;
	const uint8_t flag = args->flags;
	const bgp_size_t length = args->length;

	if (bgp_debug_update(peer, NULL, NULL, 1))
		zlog_debug(
			"%s Unknown attribute is received (type %d, length %d)",
			peer->host, type, length);

	 
	stream_forward_getp(peer->curr, length);

	 
	if (!CHECK_FLAG(flag, BGP_ATTR_FLAG_OPTIONAL)) {
		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_UNREC_ATTR,
					  args->total);
	}

	 
	if (!CHECK_FLAG(flag, BGP_ATTR_FLAG_TRANS))
		return BGP_ATTR_PARSE_PROCEED;

	 
	SET_FLAG(*startp, BGP_ATTR_FLAG_PARTIAL);

	 
	if (!attr->transit)
		attr->transit = XCALLOC(MTYPE_TRANSIT, sizeof(struct transit));

	transit = attr->transit;

	if (transit->val)
		transit->val = XREALLOC(MTYPE_TRANSIT_VAL, transit->val,
					transit->length + total);
	else
		transit->val = XMALLOC(MTYPE_TRANSIT_VAL, total);

	memcpy(transit->val + transit->length, startp, total);
	transit->length += total;

	return BGP_ATTR_PARSE_PROCEED;
}
