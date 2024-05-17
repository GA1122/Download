static bgp_attr_parse_ret_t bgp_attr_med(struct bgp_attr_parser_args *args)
{
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	const bgp_size_t length = args->length;

	 
	if (length != 4) {
		flog_err(EC_BGP_ATTR_LEN,
			 "MED attribute length isn't four [%d]", length);

		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
					  args->total);
	}

	attr->med = stream_getl(peer->curr);

	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_MULTI_EXIT_DISC);

	return BGP_ATTR_PARSE_PROCEED;
}