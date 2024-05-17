bgp_attr_local_pref(struct bgp_attr_parser_args *args)
{
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	const bgp_size_t length = args->length;

	 
	if (length != 4) {
		flog_err(EC_BGP_ATTR_LEN,
			 "LOCAL_PREF attribute length isn't 4 [%u]", length);
		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
					  args->total);
	}

	 
	if (peer->sort == BGP_PEER_EBGP) {
		stream_forward_getp(peer->curr, length);
		return BGP_ATTR_PARSE_PROCEED;
	}

	attr->local_pref = stream_getl(peer->curr);

	 
	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_LOCAL_PREF);

	return BGP_ATTR_PARSE_PROCEED;
}
