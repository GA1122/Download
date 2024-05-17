ospf6_print_lls(netdissect_options *ndo,
                const u_char *cp, const u_int len)
{
	uint16_t llsdatalen;

	if (len == 0)
		return 0;
	if (len < OSPF_LLS_HDRLEN)
		goto trunc;
	 
	ND_TCHECK2(*cp, 2);
	ND_PRINT((ndo, "\n\tLLS Checksum 0x%04x", EXTRACT_16BITS(cp)));
	cp += 2;
	 
	ND_TCHECK2(*cp, 2);
	llsdatalen = EXTRACT_16BITS(cp);
	ND_PRINT((ndo, ", Data Length %u", llsdatalen));
	if (llsdatalen < OSPF_LLS_HDRLEN || llsdatalen > len)
		goto trunc;
	cp += 2;
	 
	ND_TCHECK2(*cp, llsdatalen - OSPF_LLS_HDRLEN);
	 

	return llsdatalen;
trunc:
	return -1;
}
