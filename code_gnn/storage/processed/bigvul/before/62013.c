isakmp_print(netdissect_options *ndo,
	     const u_char *bp, u_int length,
	     const u_char *bp2)
{
	const struct isakmp *p;
	struct isakmp base;
	const u_char *ep;
	int major, minor;

#ifdef HAVE_LIBCRYPTO
	 
	if (ndo->ndo_sa_list_head == NULL) {
		if (ndo->ndo_espsecret)
			esp_print_decodesecret(ndo);
	}
#endif

	p = (const struct isakmp *)bp;
	ep = ndo->ndo_snapend;

	if ((const struct isakmp *)ep < p + 1) {
		ND_PRINT((ndo,"[|isakmp]"));
		return;
	}

	UNALIGNED_MEMCPY(&base, p, sizeof(base));

	ND_PRINT((ndo,"isakmp"));
	major = (base.vers & ISAKMP_VERS_MAJOR)
		>> ISAKMP_VERS_MAJOR_SHIFT;
	minor = (base.vers & ISAKMP_VERS_MINOR)
		>> ISAKMP_VERS_MINOR_SHIFT;

	if (ndo->ndo_vflag) {
		ND_PRINT((ndo," %d.%d", major, minor));
	}

	if (ndo->ndo_vflag) {
		ND_PRINT((ndo," msgid "));
		hexprint(ndo, (const uint8_t *)&base.msgid, sizeof(base.msgid));
	}

	if (1 < ndo->ndo_vflag) {
		ND_PRINT((ndo," cookie "));
		hexprint(ndo, (const uint8_t *)&base.i_ck, sizeof(base.i_ck));
		ND_PRINT((ndo,"->"));
		hexprint(ndo, (const uint8_t *)&base.r_ck, sizeof(base.r_ck));
	}
	ND_PRINT((ndo,":"));

	switch(major) {
	case IKEv1_MAJOR_VERSION:
		ikev1_print(ndo, bp, length, bp2, &base);
		break;

	case IKEv2_MAJOR_VERSION:
		ikev2_print(ndo, bp, length, bp2, &base);
		break;
	}
}