mobility_print(netdissect_options *ndo,
               const u_char *bp, const u_char *bp2 _U_)
{
	const struct ip6_mobility *mh;
	const u_char *ep;
	unsigned mhlen, hlen;
	uint8_t type;

	mh = (const struct ip6_mobility *)bp;

	 
	ep = ndo->ndo_snapend;

	if (!ND_TTEST(mh->ip6m_len)) {
		 
		mhlen = ep - bp;
		goto trunc;
	}
	mhlen = (mh->ip6m_len + 1) << 3;

	 

	ND_TCHECK(mh->ip6m_type);
	type = mh->ip6m_type;
	if (type <= IP6M_MAX && mhlen < ip6m_hdrlen[type]) {
		ND_PRINT((ndo, "(header length %u is too small for type %u)", mhlen, type));
		goto trunc;
	}
	ND_PRINT((ndo, "mobility: %s", tok2str(ip6m_str, "type-#%u", type)));
	switch (type) {
	case IP6M_BINDING_REQUEST:
		hlen = IP6M_MINLEN;
		break;
	case IP6M_HOME_TEST_INIT:
	case IP6M_CAREOF_TEST_INIT:
		hlen = IP6M_MINLEN;
		if (ndo->ndo_vflag) {
			ND_TCHECK_32BITS(&bp[hlen + 4]);
			ND_PRINT((ndo, " %s Init Cookie=%08x:%08x",
			       type == IP6M_HOME_TEST_INIT ? "Home" : "Care-of",
			       EXTRACT_32BITS(&bp[hlen]),
			       EXTRACT_32BITS(&bp[hlen + 4])));
		}
		hlen += 8;
		break;
	case IP6M_HOME_TEST:
	case IP6M_CAREOF_TEST:
		ND_TCHECK(mh->ip6m_data16[0]);
		ND_PRINT((ndo, " nonce id=0x%x", EXTRACT_16BITS(&mh->ip6m_data16[0])));
		hlen = IP6M_MINLEN;
		if (ndo->ndo_vflag) {
			ND_TCHECK_32BITS(&bp[hlen + 4]);
			ND_PRINT((ndo, " %s Init Cookie=%08x:%08x",
			       type == IP6M_HOME_TEST ? "Home" : "Care-of",
			       EXTRACT_32BITS(&bp[hlen]),
			       EXTRACT_32BITS(&bp[hlen + 4])));
		}
		hlen += 8;
		if (ndo->ndo_vflag) {
			ND_TCHECK_32BITS(&bp[hlen + 4]);
			ND_PRINT((ndo, " %s Keygen Token=%08x:%08x",
			       type == IP6M_HOME_TEST ? "Home" : "Care-of",
			       EXTRACT_32BITS(&bp[hlen]),
			       EXTRACT_32BITS(&bp[hlen + 4])));
		}
		hlen += 8;
		break;
	case IP6M_BINDING_UPDATE:
		ND_TCHECK(mh->ip6m_data16[0]);
		ND_PRINT((ndo, " seq#=%u", EXTRACT_16BITS(&mh->ip6m_data16[0])));
		hlen = IP6M_MINLEN;
		ND_TCHECK_16BITS(&bp[hlen]);
		if (bp[hlen] & 0xf0) {
			ND_PRINT((ndo, " "));
			if (bp[hlen] & 0x80)
				ND_PRINT((ndo, "A"));
			if (bp[hlen] & 0x40)
				ND_PRINT((ndo, "H"));
			if (bp[hlen] & 0x20)
				ND_PRINT((ndo, "L"));
			if (bp[hlen] & 0x10)
				ND_PRINT((ndo, "K"));
		}
		 
		hlen += 1;
		 
		hlen += 1;
		ND_TCHECK_16BITS(&bp[hlen]);
		 
		ND_PRINT((ndo, " lifetime=%u", EXTRACT_16BITS(&bp[hlen]) << 2));
		hlen += 2;
		break;
	case IP6M_BINDING_ACK:
		ND_TCHECK(mh->ip6m_data8[0]);
		ND_PRINT((ndo, " status=%u", mh->ip6m_data8[0]));
		ND_TCHECK(mh->ip6m_data8[1]);
		if (mh->ip6m_data8[1] & 0x80)
			ND_PRINT((ndo, " K"));
		 
		hlen = IP6M_MINLEN;
		ND_TCHECK_16BITS(&bp[hlen]);
		ND_PRINT((ndo, " seq#=%u", EXTRACT_16BITS(&bp[hlen])));
		hlen += 2;
		ND_TCHECK_16BITS(&bp[hlen]);
		 
		ND_PRINT((ndo, " lifetime=%u", EXTRACT_16BITS(&bp[hlen]) << 2));
		hlen += 2;
		break;
	case IP6M_BINDING_ERROR:
		ND_TCHECK(mh->ip6m_data8[0]);
		ND_PRINT((ndo, " status=%u", mh->ip6m_data8[0]));
		 
		hlen = IP6M_MINLEN;
		ND_TCHECK2(bp[hlen], 16);
		ND_PRINT((ndo, " homeaddr %s", ip6addr_string(ndo, &bp[hlen])));
		hlen += 16;
		break;
	default:
		ND_PRINT((ndo, " len=%u", mh->ip6m_len));
		return(mhlen);
		break;
	}
	if (ndo->ndo_vflag)
		if (mobility_opt_print(ndo, &bp[hlen], mhlen - hlen))
			goto trunc;

	return(mhlen);

 trunc:
	ND_PRINT((ndo, "%s", tstr));
	return(-1);
}