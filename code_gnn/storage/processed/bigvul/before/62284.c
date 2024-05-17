vldb_print(netdissect_options *ndo,
           register const u_char *bp, int length)
{
	int vldb_op;
	unsigned long i;

	if (length <= (int)sizeof(struct rx_header))
		return;

	if (ndo->ndo_snapend - bp + 1 <= (int)(sizeof(struct rx_header) + sizeof(int32_t))) {
		goto trunc;
	}

	 

	vldb_op = EXTRACT_32BITS(bp + sizeof(struct rx_header));

	ND_PRINT((ndo, " vldb"));

	if (is_ubik(vldb_op)) {
		ubik_print(ndo, bp);
		return;
	}
	ND_PRINT((ndo, " call %s", tok2str(vldb_req, "op#%d", vldb_op)));

	 

	bp += sizeof(struct rx_header) + 4;

	switch (vldb_op) {
		case 501:	 
		case 517:	 
			VECOUT(VLNAMEMAX);
			break;
		case 502:	 
		case 503:	 
		case 507:	 
		case 508:	 
		case 509:	 
		case 518:	 
			ND_PRINT((ndo, " volid"));
			INTOUT();
			ND_TCHECK2(bp[0], sizeof(int32_t));
			i = EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);
			if (i <= 2)
				ND_PRINT((ndo, " type %s", voltype[i]));
			break;
		case 504:	 
		case 519:	 
		case 524:	 
		case 527:	 
			STROUT(VLNAMEMAX);
			break;
		case 505:	 
			ND_PRINT((ndo, " bump"));
			INTOUT();
			break;
		case 506:	 
		case 520:	 
			ND_PRINT((ndo, " volid"));
			INTOUT();
			ND_TCHECK2(bp[0], sizeof(int32_t));
			i = EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);
			if (i <= 2)
				ND_PRINT((ndo, " type %s", voltype[i]));
			VECOUT(VLNAMEMAX);
			break;
		case 510:	 
		case 521:	 
			ND_PRINT((ndo, " index"));
			INTOUT();
			break;
		default:
			;
	}

	return;

trunc:
	ND_PRINT((ndo, " [|vldb]"));
}
