ip_print_demux(netdissect_options *ndo,
	       struct ip_print_demux_state *ipds)
{
	const char *p_name;

again:
	switch (ipds->nh) {

	case IPPROTO_AH:
		if (!ND_TTEST(*ipds->cp)) {
			ND_PRINT((ndo, "[|AH]"));
			break;
		}
		ipds->nh = *ipds->cp;
		ipds->advance = ah_print(ndo, ipds->cp);
		if (ipds->advance <= 0)
			break;
		ipds->cp += ipds->advance;
		ipds->len -= ipds->advance;
		goto again;

	case IPPROTO_ESP:
	{
		int enh, padlen;
		ipds->advance = esp_print(ndo, ipds->cp, ipds->len,
				    (const u_char *)ipds->ip,
				    &enh, &padlen);
		if (ipds->advance <= 0)
			break;
		ipds->cp += ipds->advance;
		ipds->len -= ipds->advance + padlen;
		ipds->nh = enh & 0xff;
		goto again;
	}

	case IPPROTO_IPCOMP:
	{
		ipcomp_print(ndo, ipds->cp);
		 
		break;
	}

	case IPPROTO_SCTP:
		sctp_print(ndo, ipds->cp, (const u_char *)ipds->ip, ipds->len);
		break;

	case IPPROTO_DCCP:
		dccp_print(ndo, ipds->cp, (const u_char *)ipds->ip, ipds->len);
		break;

	case IPPROTO_TCP:
		 
		tcp_print(ndo, ipds->cp, ipds->len, (const u_char *)ipds->ip,
			  ipds->off & (IP_MF|IP_OFFMASK));
		break;

	case IPPROTO_UDP:
		 
		udp_print(ndo, ipds->cp, ipds->len, (const u_char *)ipds->ip,
			  ipds->off & (IP_MF|IP_OFFMASK));
		break;

	case IPPROTO_ICMP:
		 
		icmp_print(ndo, ipds->cp, ipds->len, (const u_char *)ipds->ip,
			   ipds->off & (IP_MF|IP_OFFMASK));
		break;

	case IPPROTO_PIGP:
		 
		igrp_print(ndo, ipds->cp, ipds->len);
		break;

	case IPPROTO_EIGRP:
		eigrp_print(ndo, ipds->cp, ipds->len);
		break;

	case IPPROTO_ND:
		ND_PRINT((ndo, " nd %d", ipds->len));
		break;

	case IPPROTO_EGP:
		egp_print(ndo, ipds->cp, ipds->len);
		break;

	case IPPROTO_OSPF:
		ospf_print(ndo, ipds->cp, ipds->len, (const u_char *)ipds->ip);
		break;

	case IPPROTO_IGMP:
		igmp_print(ndo, ipds->cp, ipds->len);
		break;

	case IPPROTO_IPV4:
		 
		ip_print(ndo, ipds->cp, ipds->len);
		if (! ndo->ndo_vflag) {
			ND_PRINT((ndo, " (ipip-proto-4)"));
			return;
		}
		break;

	case IPPROTO_IPV6:
		 
		ip6_print(ndo, ipds->cp, ipds->len);
		break;

	case IPPROTO_RSVP:
		rsvp_print(ndo, ipds->cp, ipds->len);
		break;

	case IPPROTO_GRE:
		 
		gre_print(ndo, ipds->cp, ipds->len);
		break;

	case IPPROTO_MOBILE:
		mobile_print(ndo, ipds->cp, ipds->len);
		break;

	case IPPROTO_PIM:
		pim_print(ndo, ipds->cp, ipds->len, (const u_char *)ipds->ip);
		break;

	case IPPROTO_VRRP:
		if (ndo->ndo_packettype == PT_CARP) {
			if (ndo->ndo_vflag)
				ND_PRINT((ndo, "carp %s > %s: ",
					     ipaddr_string(ndo, &ipds->ip->ip_src),
					     ipaddr_string(ndo, &ipds->ip->ip_dst)));
			carp_print(ndo, ipds->cp, ipds->len, ipds->ip->ip_ttl);
		} else {
			if (ndo->ndo_vflag)
				ND_PRINT((ndo, "vrrp %s > %s: ",
					     ipaddr_string(ndo, &ipds->ip->ip_src),
					     ipaddr_string(ndo, &ipds->ip->ip_dst)));
			vrrp_print(ndo, ipds->cp, ipds->len,
				(const u_char *)ipds->ip, ipds->ip->ip_ttl);
		}
		break;

	case IPPROTO_PGM:
		pgm_print(ndo, ipds->cp, ipds->len, (const u_char *)ipds->ip);
		break;

	default:
		if (ndo->ndo_nflag==0 && (p_name = netdb_protoname(ipds->nh)) != NULL)
			ND_PRINT((ndo, " %s", p_name));
		else
			ND_PRINT((ndo, " ip-proto-%d", ipds->nh));
		ND_PRINT((ndo, " %d", ipds->len));
		break;
	}
}
