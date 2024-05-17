isakmp_rfc3948_print(netdissect_options *ndo,
		     const u_char *bp, u_int length,
		     const u_char *bp2)
{
	ND_TCHECK(bp[0]);
	if(length == 1 && bp[0]==0xff) {
		ND_PRINT((ndo, "isakmp-nat-keep-alive"));
		return;
	}

	if(length < 4) {
		goto trunc;
	}
	ND_TCHECK(bp[3]);

	 
	if(bp[0]==0 && bp[1]==0 && bp[2]==0 && bp[3]==0) {
		ND_PRINT((ndo, "NONESP-encap: "));
		isakmp_print(ndo, bp+4, length-4, bp2);
		return;
	}

	 
	{
		int nh, enh, padlen;
		int advance;

		ND_PRINT((ndo, "UDP-encap: "));

		advance = esp_print(ndo, bp, length, bp2, &enh, &padlen);
		if(advance <= 0)
			return;

		bp += advance;
		length -= advance + padlen;
		nh = enh & 0xff;

		ip_print_inner(ndo, bp, length, nh, bp2);
		return;
	}

trunc:
	ND_PRINT((ndo,"[|isakmp]"));
	return;
}