bgp_attr_pmsi_tunnel(struct bgp_attr_parser_args *args)
{
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	const bgp_size_t length = args->length;
	uint8_t tnl_type;

	 
	if (length < 2) {
		flog_err(EC_BGP_ATTR_LEN, "Bad PMSI tunnel attribute length %d",
			 length);
		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
					  args->total);
	}
	stream_getc(peer->curr);  
	tnl_type = stream_getc(peer->curr);
	if (tnl_type > PMSI_TNLTYPE_MAX) {
		flog_err(EC_BGP_ATTR_PMSI_TYPE,
			 "Invalid PMSI tunnel attribute type %d", tnl_type);
		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_OPT_ATTR_ERR,
					  args->total);
	}
	if (tnl_type == PMSI_TNLTYPE_INGR_REPL) {
		if (length != 9) {
			flog_err(EC_BGP_ATTR_PMSI_LEN,
				 "Bad PMSI tunnel attribute length %d for IR",
				 length);
			return bgp_attr_malformed(
				args, BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
				args->total);
		}
	}

	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_PMSI_TUNNEL);
	attr->pmsi_tnl_type = tnl_type;

	 
	stream_forward_getp(peer->curr, length - 2);

	return BGP_ATTR_PARSE_PROCEED;
}