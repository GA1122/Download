ip6_finddst(netdissect_options *ndo, struct in6_addr *dst,
            const struct ip6_hdr *ip6)
{
	const u_char *cp;
	int advance;
	u_int nh;
	const void *dst_addr;
	const struct ip6_rthdr *dp;
	const struct ip6_rthdr0 *dp0;
	const struct in6_addr *addr;
	int i, len;

	cp = (const u_char *)ip6;
	advance = sizeof(struct ip6_hdr);
	nh = ip6->ip6_nxt;
	dst_addr = (const void *)&ip6->ip6_dst;

	while (cp < ndo->ndo_snapend) {
		cp += advance;

		switch (nh) {

		case IPPROTO_HOPOPTS:
		case IPPROTO_DSTOPTS:
		case IPPROTO_MOBILITY_OLD:
		case IPPROTO_MOBILITY:
			 
			ND_TCHECK2(*cp, 2);
			advance = (int)((*(cp + 1) + 1) << 3);
			nh = *cp;
			break;

		case IPPROTO_FRAGMENT:
			 
			ND_TCHECK2(*cp, 1);
			advance = sizeof(struct ip6_frag);
			nh = *cp;
			break;

		case IPPROTO_ROUTING:
			 
			dp = (const struct ip6_rthdr *)cp;
			ND_TCHECK(*dp);
			len = dp->ip6r_len;
			switch (dp->ip6r_type) {

			case IPV6_RTHDR_TYPE_0:
			case IPV6_RTHDR_TYPE_2:		 
				dp0 = (const struct ip6_rthdr0 *)dp;
				if (len % 2 == 1)
					goto trunc;
				len >>= 1;
				addr = &dp0->ip6r0_addr[0];
				for (i = 0; i < len; i++) {
					if ((const u_char *)(addr + 1) > ndo->ndo_snapend)
						goto trunc;

					dst_addr = (const void *)addr;
					addr++;
				}
				break;

			default:
				break;
			}

			 
			goto done;

		case IPPROTO_AH:
		case IPPROTO_ESP:
		case IPPROTO_IPCOMP:
		default:
			 
			goto done;
		}
	}

done:
trunc:
	UNALIGNED_MEMCPY(dst, dst_addr, sizeof(struct in6_addr));
}
