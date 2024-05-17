ip_print(netdissect_options *ndo,
	 const u_char *bp,
	 u_int length)
{
	struct ip_print_demux_state  ipd;
	struct ip_print_demux_state *ipds=&ipd;
	const u_char *ipend;
	u_int hlen;
	struct cksum_vec vec[1];
	uint16_t sum, ip_sum;
	const char *p_name;

	ipds->ip = (const struct ip *)bp;
	ND_TCHECK(ipds->ip->ip_vhl);
	if (IP_V(ipds->ip) != 4) {  
	    if (IP_V(ipds->ip) == 6)
	      ND_PRINT((ndo, "IP6, wrong link-layer encapsulation "));
	    else
	      ND_PRINT((ndo, "IP%u ", IP_V(ipds->ip)));
	    return;
	}
	if (!ndo->ndo_eflag)
		ND_PRINT((ndo, "IP "));

	ND_TCHECK(*ipds->ip);
	if (length < sizeof (struct ip)) {
		ND_PRINT((ndo, "truncated-ip %u", length));
		return;
	}
	hlen = IP_HL(ipds->ip) * 4;
	if (hlen < sizeof (struct ip)) {
		ND_PRINT((ndo, "bad-hlen %u", hlen));
		return;
	}

	ipds->len = EXTRACT_16BITS(&ipds->ip->ip_len);
	if (length < ipds->len)
		ND_PRINT((ndo, "truncated-ip - %u bytes missing! ",
			ipds->len - length));
	if (ipds->len < hlen) {
#ifdef GUESS_TSO
            if (ipds->len) {
                ND_PRINT((ndo, "bad-len %u", ipds->len));
                return;
            }
            else {
                 
                ipds->len = length;
            }
#else
            ND_PRINT((ndo, "bad-len %u", ipds->len));
            return;
#endif  
	}

	 
	ipend = bp + ipds->len;
	if (ipend < ndo->ndo_snapend)
		ndo->ndo_snapend = ipend;

	ipds->len -= hlen;

	ipds->off = EXTRACT_16BITS(&ipds->ip->ip_off);

        if (ndo->ndo_vflag) {
            ND_PRINT((ndo, "(tos 0x%x", (int)ipds->ip->ip_tos));
             
            switch (ipds->ip->ip_tos & 0x03) {

            case 0:
                break;

            case 1:
                ND_PRINT((ndo, ",ECT(1)"));
                break;

            case 2:
                ND_PRINT((ndo, ",ECT(0)"));
                break;

            case 3:
                ND_PRINT((ndo, ",CE"));
                break;
            }

            if (ipds->ip->ip_ttl >= 1)
                ND_PRINT((ndo, ", ttl %u", ipds->ip->ip_ttl));

	     

	    ND_PRINT((ndo, ", id %u, offset %u, flags [%s], proto %s (%u)",
                         EXTRACT_16BITS(&ipds->ip->ip_id),
                         (ipds->off & 0x1fff) * 8,
                         bittok2str(ip_frag_values, "none", ipds->off&0xe000),
                         tok2str(ipproto_values,"unknown",ipds->ip->ip_p),
                         ipds->ip->ip_p));

            ND_PRINT((ndo, ", length %u", EXTRACT_16BITS(&ipds->ip->ip_len)));

            if ((hlen - sizeof(struct ip)) > 0) {
                ND_PRINT((ndo, ", options ("));
                ip_optprint(ndo, (const u_char *)(ipds->ip + 1), hlen - sizeof(struct ip));
                ND_PRINT((ndo, ")"));
            }

	    if (!ndo->ndo_Kflag && (const u_char *)ipds->ip + hlen <= ndo->ndo_snapend) {
	        vec[0].ptr = (const uint8_t *)(const void *)ipds->ip;
	        vec[0].len = hlen;
	        sum = in_cksum(vec, 1);
		if (sum != 0) {
		    ip_sum = EXTRACT_16BITS(&ipds->ip->ip_sum);
		    ND_PRINT((ndo, ", bad cksum %x (->%x)!", ip_sum,
			     in_cksum_shouldbe(ip_sum, sum)));
		}
	    }

		ND_PRINT((ndo, ")\n    "));
	}

	 
	if ((ipds->off & 0x1fff) == 0) {
		ipds->cp = (const u_char *)ipds->ip + hlen;
		ipds->nh = ipds->ip->ip_p;

		if (ipds->nh != IPPROTO_TCP && ipds->nh != IPPROTO_UDP &&
		    ipds->nh != IPPROTO_SCTP && ipds->nh != IPPROTO_DCCP) {
			ND_PRINT((ndo, "%s > %s: ",
				     ipaddr_string(ndo, &ipds->ip->ip_src),
				     ipaddr_string(ndo, &ipds->ip->ip_dst)));
		}
		ip_print_demux(ndo, ipds);
	} else {
		 
		if (ndo->ndo_qflag > 1)
			return;

		 
		ND_PRINT((ndo, "%s > %s:", ipaddr_string(ndo, &ipds->ip->ip_src),
		          ipaddr_string(ndo, &ipds->ip->ip_dst)));
		if (!ndo->ndo_nflag && (p_name = netdb_protoname(ipds->ip->ip_p)) != NULL)
			ND_PRINT((ndo, " %s", p_name));
		else
			ND_PRINT((ndo, " ip-proto-%d", ipds->ip->ip_p));
	}
	return;

trunc:
	ND_PRINT((ndo, "%s", tstr));
	return;
}
