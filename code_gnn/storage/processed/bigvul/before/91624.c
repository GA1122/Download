static int bgp_attr_check(struct peer *peer, struct attr *attr)
{
	uint8_t type = 0;

	 
	if (CHECK_FLAG(peer->cap, PEER_CAP_RESTART_RCV) && !attr->flag)
		return BGP_ATTR_PARSE_PROCEED;

	 
	if ((!CHECK_FLAG(attr->flag, ATTR_FLAG_BIT(BGP_ATTR_MP_REACH_NLRI)) &&
	     CHECK_FLAG(attr->flag, ATTR_FLAG_BIT(BGP_ATTR_MP_UNREACH_NLRI))))
		return BGP_ATTR_PARSE_PROCEED;

	if (!CHECK_FLAG(attr->flag, ATTR_FLAG_BIT(BGP_ATTR_ORIGIN)))
		type = BGP_ATTR_ORIGIN;

	if (!CHECK_FLAG(attr->flag, ATTR_FLAG_BIT(BGP_ATTR_AS_PATH)))
		type = BGP_ATTR_AS_PATH;

	 
	if (!CHECK_FLAG(attr->flag, ATTR_FLAG_BIT(BGP_ATTR_NEXT_HOP))
	    && !CHECK_FLAG(attr->flag, ATTR_FLAG_BIT(BGP_ATTR_MP_REACH_NLRI)))
		type = BGP_ATTR_NEXT_HOP;

	if (peer->sort == BGP_PEER_IBGP
	    && !CHECK_FLAG(attr->flag, ATTR_FLAG_BIT(BGP_ATTR_LOCAL_PREF)))
		type = BGP_ATTR_LOCAL_PREF;

	if (type) {
		flog_warn(EC_BGP_MISSING_ATTRIBUTE,
			  "%s Missing well-known attribute %s.", peer->host,
			  lookup_msg(attr_str, type, NULL));
		bgp_notify_send_with_data(peer, BGP_NOTIFY_UPDATE_ERR,
					  BGP_NOTIFY_UPDATE_MISS_ATTR, &type,
					  1);
		return BGP_ATTR_PARSE_ERROR;
	}
	return BGP_ATTR_PARSE_PROCEED;
}
