static bgp_attr_parse_ret_t bgp_attr_psid_sub(int32_t type,
					      int32_t length,
					      struct bgp_attr_parser_args *args,
					      struct bgp_nlri *mp_update)
{
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	uint32_t label_index;
	struct in6_addr ipv6_sid;
	uint32_t srgb_base;
	uint32_t srgb_range;
	int srgb_count;

	if (type == BGP_PREFIX_SID_LABEL_INDEX) {
		if (length != BGP_PREFIX_SID_LABEL_INDEX_LENGTH) {
			flog_err(
				EC_BGP_ATTR_LEN,
				"Prefix SID label index length is %d instead of %d",
				length, BGP_PREFIX_SID_LABEL_INDEX_LENGTH);
			return bgp_attr_malformed(args,
						  BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
						  args->total);
		}

		 
		stream_getc(peer->curr);
		stream_getw(peer->curr);

		 
		label_index = stream_getl(peer->curr);
		if (label_index == BGP_INVALID_LABEL_INDEX)
			return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_OPT_ATTR_ERR,
						  args->total);

		 
		attr->label_index = label_index;

		 
		if (!mp_update->length
		    || mp_update->safi != SAFI_LABELED_UNICAST)
			attr->label_index = BGP_INVALID_LABEL_INDEX;
	}

	 
	else if (type == BGP_PREFIX_SID_IPV6) {
		if (length != BGP_PREFIX_SID_IPV6_LENGTH) {
			flog_err(EC_BGP_ATTR_LEN,
				 "Prefix SID IPv6 length is %d instead of %d",
				 length, BGP_PREFIX_SID_IPV6_LENGTH);
			return bgp_attr_malformed(args,
						  BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
						  args->total);
		}

		 
		stream_getc(peer->curr);
		stream_getw(peer->curr);

		stream_get(&ipv6_sid, peer->curr, 16);
	}

	 
	else if (type == BGP_PREFIX_SID_ORIGINATOR_SRGB) {
		 
		stream_getw(peer->curr);

		length -= 2;

		if (length % BGP_PREFIX_SID_ORIGINATOR_SRGB_LENGTH) {
			flog_err(
				EC_BGP_ATTR_LEN,
				"Prefix SID Originator SRGB length is %d, it must be a multiple of %d ",
				length, BGP_PREFIX_SID_ORIGINATOR_SRGB_LENGTH);
			return bgp_attr_malformed(
				args, BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
				args->total);
		}

		srgb_count = length / BGP_PREFIX_SID_ORIGINATOR_SRGB_LENGTH;

		for (int i = 0; i < srgb_count; i++) {
			stream_get(&srgb_base, peer->curr, 3);
			stream_get(&srgb_range, peer->curr, 3);
		}
	}

	return BGP_ATTR_PARSE_PROCEED;
}