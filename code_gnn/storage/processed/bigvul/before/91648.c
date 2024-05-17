bgp_attr_prefix_sid(int32_t tlength, struct bgp_attr_parser_args *args,
		    struct bgp_nlri *mp_update)
{
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	bgp_attr_parse_ret_t ret;

	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_PREFIX_SID);

	while (tlength) {
		int32_t type, length;

		type = stream_getc(peer->curr);
		length = stream_getw(peer->curr);

		ret = bgp_attr_psid_sub(type, length, args, mp_update);

		if (ret != BGP_ATTR_PARSE_PROCEED)
			return ret;
		 
		tlength -= length + 3;

		if (tlength < 0) {
			flog_err(
				EC_BGP_ATTR_LEN,
				"Prefix SID internal length %d causes us to read beyond the total Prefix SID length",
				length);
			return bgp_attr_malformed(args,
						  BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
						  args->total);
		}
	}

	return BGP_ATTR_PARSE_PROCEED;
}