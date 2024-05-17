bgp_attr_ext_communities(struct bgp_attr_parser_args *args)
{
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	const bgp_size_t length = args->length;
	uint8_t sticky = 0;

	if (length == 0) {
		attr->ecommunity = NULL;
		 
		return BGP_ATTR_PARSE_PROCEED;
	}

	attr->ecommunity =
		ecommunity_parse((uint8_t *)stream_pnt(peer->curr), length);
	 
	stream_forward_getp(peer->curr, length);

	if (!attr->ecommunity)
		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_OPT_ATTR_ERR,
					  args->total);

	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_EXT_COMMUNITIES);

	 
	attr->mm_seqnum = bgp_attr_mac_mobility_seqnum(attr, &sticky);
	attr->sticky = sticky;

	 
	attr->default_gw = bgp_attr_default_gw(attr);

	 
	if (attr->default_gw)
		attr->router_flag = 1;

	 
	bgp_attr_evpn_na_flag(attr, &attr->router_flag);

	 
	bgp_attr_rmac(attr, &attr->rmac);

	return BGP_ATTR_PARSE_PROCEED;
}
