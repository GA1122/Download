static bgp_attr_parse_ret_t bgp_attr_nexthop(struct bgp_attr_parser_args *args)
{
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	const bgp_size_t length = args->length;

	in_addr_t nexthop_h, nexthop_n;

	 
	if (length != 4) {
		flog_err(EC_BGP_ATTR_LEN,
			 "Nexthop attribute length isn't four [%d]", length);

		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
					  args->total);
	}

	 
	nexthop_n = stream_get_ipv4(peer->curr);
	nexthop_h = ntohl(nexthop_n);
	if ((IPV4_NET0(nexthop_h) || IPV4_NET127(nexthop_h)
	     || IPV4_CLASS_DE(nexthop_h))
	    && !BGP_DEBUG(
		       allow_martians,
		       ALLOW_MARTIANS))  
	{
		char buf[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &nexthop_n, buf, INET_ADDRSTRLEN);
		flog_err(EC_BGP_ATTR_MARTIAN_NH, "Martian nexthop %s", buf);
		return bgp_attr_malformed(
			args, BGP_NOTIFY_UPDATE_INVAL_NEXT_HOP, args->total);
	}

	attr->nexthop.s_addr = nexthop_n;
	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_NEXT_HOP);

	return BGP_ATTR_PARSE_PROCEED;
}
