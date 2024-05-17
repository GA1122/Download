int bgp_mp_reach_parse(struct bgp_attr_parser_args *args,
		       struct bgp_nlri *mp_update)
{
	iana_afi_t pkt_afi;
	afi_t afi;
	iana_safi_t pkt_safi;
	safi_t safi;
	bgp_size_t nlri_len;
	size_t start;
	struct stream *s;
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	const bgp_size_t length = args->length;

	 
	s = BGP_INPUT(peer);
	start = stream_get_getp(s);

 
#define BGP_MP_REACH_MIN_SIZE 5
#define LEN_LEFT	(length - (stream_get_getp(s) - start))
	if ((length > STREAM_READABLE(s)) || (length < BGP_MP_REACH_MIN_SIZE)) {
		zlog_info("%s: %s sent invalid length, %lu", __func__,
			  peer->host, (unsigned long)length);
		return BGP_ATTR_PARSE_ERROR_NOTIFYPLS;
	}

	 
	pkt_afi = stream_getw(s);
	pkt_safi = stream_getc(s);

	 
	if (bgp_map_afi_safi_iana2int(pkt_afi, pkt_safi, &afi, &safi)) {
		 
		if (bgp_debug_update(peer, NULL, NULL, 0))
			zlog_debug(
				"%s: MP_REACH received AFI %u or SAFI %u is unrecognized",
				peer->host, pkt_afi, pkt_safi);
		return BGP_ATTR_PARSE_ERROR;
	}

	 
	attr->mp_nexthop_len = stream_getc(s);

	if (LEN_LEFT < attr->mp_nexthop_len) {
		zlog_info(
			"%s: %s, MP nexthop length, %u, goes past end of attribute",
			__func__, peer->host, attr->mp_nexthop_len);
		return BGP_ATTR_PARSE_ERROR_NOTIFYPLS;
	}

	 
	switch (attr->mp_nexthop_len) {
	case 0:
		if (safi != SAFI_FLOWSPEC) {
			zlog_info("%s: (%s) Wrong multiprotocol next hop length: %d",
				  __func__, peer->host, attr->mp_nexthop_len);
			return BGP_ATTR_PARSE_ERROR_NOTIFYPLS;
		}
		break;
	case BGP_ATTR_NHLEN_VPNV4:
		stream_getl(s);  
		stream_getl(s);  
				 
	 
	case BGP_ATTR_NHLEN_IPV4:
		stream_get(&attr->mp_nexthop_global_in, s, IPV4_MAX_BYTELEN);
		 
		if (attr->nexthop.s_addr == 0)
			memcpy(&attr->nexthop.s_addr,
			       &attr->mp_nexthop_global_in, IPV4_MAX_BYTELEN);
		break;
	case BGP_ATTR_NHLEN_IPV6_GLOBAL:
	case BGP_ATTR_NHLEN_VPNV6_GLOBAL:
		if (attr->mp_nexthop_len == BGP_ATTR_NHLEN_VPNV6_GLOBAL) {
			stream_getl(s);  
			stream_getl(s);  
		}
		stream_get(&attr->mp_nexthop_global, s, IPV6_MAX_BYTELEN);
		if (IN6_IS_ADDR_LINKLOCAL(&attr->mp_nexthop_global)) {
			if (!peer->nexthop.ifp) {
				zlog_warn("%s: interface not set appropriately to handle some attributes",
					  peer->host);
				return BGP_ATTR_PARSE_WITHDRAW;
			}
			attr->nh_ifindex = peer->nexthop.ifp->ifindex;
		}
		break;
	case BGP_ATTR_NHLEN_IPV6_GLOBAL_AND_LL:
	case BGP_ATTR_NHLEN_VPNV6_GLOBAL_AND_LL:
		if (attr->mp_nexthop_len
		    == BGP_ATTR_NHLEN_VPNV6_GLOBAL_AND_LL) {
			stream_getl(s);  
			stream_getl(s);  
		}
		stream_get(&attr->mp_nexthop_global, s, IPV6_MAX_BYTELEN);
		if (IN6_IS_ADDR_LINKLOCAL(&attr->mp_nexthop_global)) {
			if (!peer->nexthop.ifp) {
				zlog_warn("%s: interface not set appropriately to handle some attributes",
					  peer->host);
				return BGP_ATTR_PARSE_WITHDRAW;
			}
			attr->nh_ifindex = peer->nexthop.ifp->ifindex;
		}
		if (attr->mp_nexthop_len
		    == BGP_ATTR_NHLEN_VPNV6_GLOBAL_AND_LL) {
			stream_getl(s);  
			stream_getl(s);  
		}
		stream_get(&attr->mp_nexthop_local, s, IPV6_MAX_BYTELEN);
		if (!IN6_IS_ADDR_LINKLOCAL(&attr->mp_nexthop_local)) {
			char buf1[INET6_ADDRSTRLEN];
			char buf2[INET6_ADDRSTRLEN];

			if (bgp_debug_update(peer, NULL, NULL, 1))
				zlog_debug(
					"%s rcvd nexthops %s, %s -- ignoring non-LL value",
					peer->host,
					inet_ntop(AF_INET6,
						  &attr->mp_nexthop_global,
						  buf1, INET6_ADDRSTRLEN),
					inet_ntop(AF_INET6,
						  &attr->mp_nexthop_local, buf2,
						  INET6_ADDRSTRLEN));

			attr->mp_nexthop_len = IPV6_MAX_BYTELEN;
		}
		if (!peer->nexthop.ifp) {
			zlog_warn("%s: Interface not set appropriately to handle this some attributes",
				  peer->host);
			return BGP_ATTR_PARSE_WITHDRAW;
		}
		attr->nh_lla_ifindex = peer->nexthop.ifp->ifindex;
		break;
	default:
		zlog_info("%s: (%s) Wrong multiprotocol next hop length: %d",
			  __func__, peer->host, attr->mp_nexthop_len);
		return BGP_ATTR_PARSE_ERROR_NOTIFYPLS;
	}

	if (!LEN_LEFT) {
		zlog_info("%s: (%s) Failed to read SNPA and NLRI(s)", __func__,
			  peer->host);
		return BGP_ATTR_PARSE_ERROR_NOTIFYPLS;
	}

	{
		uint8_t val;
		if ((val = stream_getc(s)))
			flog_warn(
				EC_BGP_DEFUNCT_SNPA_LEN,
				"%s sent non-zero value, %u, for defunct SNPA-length field",
				peer->host, val);
	}

	 
	nlri_len = LEN_LEFT;
	if (nlri_len > STREAM_READABLE(s)) {
		zlog_info("%s: (%s) Failed to read NLRI", __func__, peer->host);
		return BGP_ATTR_PARSE_ERROR_NOTIFYPLS;
	}

	if (!nlri_len) {
		zlog_info("%s: (%s) No Reachability, Treating as a EOR marker",
			  __func__, peer->host);

		mp_update->afi = afi;
		mp_update->safi = safi;
		return BGP_ATTR_PARSE_EOR;
	}

	mp_update->afi = afi;
	mp_update->safi = safi;
	mp_update->nlri = stream_pnt(s);
	mp_update->length = nlri_len;

	stream_forward_getp(s, nlri_len);

	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_MP_REACH_NLRI);

	return BGP_ATTR_PARSE_PROCEED;
#undef LEN_LEFT
}