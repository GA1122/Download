bgp_attr_originator_id(struct bgp_attr_parser_args *args)
{
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	const bgp_size_t length = args->length;

	 
	if (length != 4) {
		flog_err(EC_BGP_ATTR_LEN, "Bad originator ID length %d",
			 length);

		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
					  args->total);
	}

	attr->originator_id.s_addr = stream_get_ipv4(peer->curr);

	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_ORIGINATOR_ID);

	return BGP_ATTR_PARSE_PROCEED;
}