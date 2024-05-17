get_upperlayer(netdissect_options *ndo, const u_char *bp, u_int *prot)
{
	const u_char *ep;
	const struct ip6_hdr *ip6 = (const struct ip6_hdr *)bp;
	const struct udphdr *uh;
	const struct ip6_hbh *hbh;
	const struct ip6_frag *fragh;
	const struct ah *ah;
	u_int nh;
	int hlen;

	 
	ep = ndo->ndo_snapend;

	if (!ND_TTEST(ip6->ip6_nxt))
		return NULL;

	nh = ip6->ip6_nxt;
	hlen = sizeof(struct ip6_hdr);

	while (bp < ep) {
		bp += hlen;

		switch(nh) {
		case IPPROTO_UDP:
		case IPPROTO_TCP:
			uh = (const struct udphdr *)bp;
			if (ND_TTEST(uh->uh_dport)) {
				*prot = nh;
				return(uh);
			}
			else
				return(NULL);
			 

		case IPPROTO_HOPOPTS:
		case IPPROTO_DSTOPTS:
		case IPPROTO_ROUTING:
			hbh = (const struct ip6_hbh *)bp;
			if (!ND_TTEST(hbh->ip6h_len))
				return(NULL);
			nh = hbh->ip6h_nxt;
			hlen = (hbh->ip6h_len + 1) << 3;
			break;

		case IPPROTO_FRAGMENT:  
			fragh = (const struct ip6_frag *)bp;
			if (!ND_TTEST(fragh->ip6f_offlg))
				return(NULL);
			 
			if ((EXTRACT_16BITS(&fragh->ip6f_offlg) & IP6F_OFF_MASK) != 0)
				return(NULL);
			nh = fragh->ip6f_nxt;
			hlen = sizeof(struct ip6_frag);
			break;

		case IPPROTO_AH:
			ah = (const struct ah *)bp;
			if (!ND_TTEST(ah->ah_len))
				return(NULL);
			nh = ah->ah_nxt;
			hlen = (ah->ah_len + 2) << 2;
			break;

		default:	 
			*prot = nh;  
			return(NULL);
		}
	}

	return(NULL);		 
}
