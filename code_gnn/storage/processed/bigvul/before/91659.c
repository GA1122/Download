size_t bgp_packet_mpattr_start(struct stream *s, struct peer *peer, afi_t afi,
			       safi_t safi, struct bpacket_attr_vec_arr *vecarr,
			       struct attr *attr)
{
	size_t sizep;
	iana_afi_t pkt_afi;
	iana_safi_t pkt_safi;
	afi_t nh_afi;

	 
	stream_putc(s, BGP_ATTR_FLAG_OPTIONAL | BGP_ATTR_FLAG_EXTLEN);
	stream_putc(s, BGP_ATTR_MP_REACH_NLRI);
	sizep = stream_get_endp(s);
	stream_putw(s, 0);  


	 
	bgp_map_afi_safi_int2iana(afi, safi, &pkt_afi, &pkt_safi);

	stream_putw(s, pkt_afi);   
	stream_putc(s, pkt_safi);  

	 
	if (afi == AFI_IP
	    && (safi == SAFI_UNICAST || safi == SAFI_LABELED_UNICAST))
		nh_afi = peer_cap_enhe(peer, afi, safi) ? AFI_IP6 : AFI_IP;
	else
		nh_afi = BGP_NEXTHOP_AFI_FROM_NHLEN(attr->mp_nexthop_len);

	 
	bpacket_attr_vec_arr_set_vec(vecarr, BGP_ATTR_VEC_NH, s, attr);
	switch (nh_afi) {
	case AFI_IP:
		switch (safi) {
		case SAFI_UNICAST:
		case SAFI_MULTICAST:
		case SAFI_LABELED_UNICAST:
			stream_putc(s, 4);
			stream_put_ipv4(s, attr->nexthop.s_addr);
			break;
		case SAFI_MPLS_VPN:
			stream_putc(s, 12);
			stream_putl(s, 0);  
			stream_putl(s, 0);
			stream_put(s, &attr->mp_nexthop_global_in, 4);
			break;
		case SAFI_ENCAP:
		case SAFI_EVPN:
			stream_putc(s, 4);
			stream_put(s, &attr->mp_nexthop_global_in, 4);
			break;
		case SAFI_FLOWSPEC:
			stream_putc(s, 0);  
		default:
			break;
		}
		break;
	case AFI_IP6:
		switch (safi) {
		case SAFI_UNICAST:
		case SAFI_MULTICAST:
		case SAFI_LABELED_UNICAST:
		case SAFI_EVPN: {
			if (attr->mp_nexthop_len
			    == BGP_ATTR_NHLEN_IPV6_GLOBAL_AND_LL) {
				stream_putc(s,
					    BGP_ATTR_NHLEN_IPV6_GLOBAL_AND_LL);
				stream_put(s, &attr->mp_nexthop_global,
					   IPV6_MAX_BYTELEN);
				stream_put(s, &attr->mp_nexthop_local,
					   IPV6_MAX_BYTELEN);
			} else {
				stream_putc(s, IPV6_MAX_BYTELEN);
				stream_put(s, &attr->mp_nexthop_global,
					   IPV6_MAX_BYTELEN);
			}
		} break;
		case SAFI_MPLS_VPN: {
			if (attr->mp_nexthop_len
			    == BGP_ATTR_NHLEN_IPV6_GLOBAL) {
				stream_putc(s, 24);
				stream_putl(s, 0);  
				stream_putl(s, 0);
				stream_put(s, &attr->mp_nexthop_global,
					   IPV6_MAX_BYTELEN);
			} else if (attr->mp_nexthop_len
				   == BGP_ATTR_NHLEN_IPV6_GLOBAL_AND_LL) {
				stream_putc(s, 48);
				stream_putl(s, 0);  
				stream_putl(s, 0);
				stream_put(s, &attr->mp_nexthop_global,
					   IPV6_MAX_BYTELEN);
				stream_putl(s, 0);  
				stream_putl(s, 0);
				stream_put(s, &attr->mp_nexthop_local,
					   IPV6_MAX_BYTELEN);
			}
		} break;
		case SAFI_ENCAP:
			stream_putc(s, IPV6_MAX_BYTELEN);
			stream_put(s, &attr->mp_nexthop_global,
				   IPV6_MAX_BYTELEN);
			break;
		case SAFI_FLOWSPEC:
			stream_putc(s, 0);  
		default:
			break;
		}
		break;
	default:
		if (safi != SAFI_FLOWSPEC)
			flog_err(
				EC_BGP_ATTR_NH_SEND_LEN,
				"Bad nexthop when sending to %s, AFI %u SAFI %u nhlen %d",
				peer->host, afi, safi, attr->mp_nexthop_len);
		break;
	}

	 
	stream_putc(s, 0);
	return sizep;
}