prot_print(netdissect_options *ndo,
           register const u_char *bp, int length)
{
	unsigned long i;
	int pt_op;

	if (length <= (int)sizeof(struct rx_header))
		return;

	if (ndo->ndo_snapend - bp + 1 <= (int)(sizeof(struct rx_header) + sizeof(int32_t))) {
		goto trunc;
	}

	 

	pt_op = EXTRACT_32BITS(bp + sizeof(struct rx_header));

	ND_PRINT((ndo, " pt"));

	if (is_ubik(pt_op)) {
		ubik_print(ndo, bp);
		return;
	}

	ND_PRINT((ndo, " call %s", tok2str(pt_req, "op#%d", pt_op)));

	 

	bp += sizeof(struct rx_header) + 4;

	switch (pt_op) {
		case 500:	 
			STROUT(PRNAMEMAX);
			ND_PRINT((ndo, " id"));
			INTOUT();
			ND_PRINT((ndo, " oldid"));
			INTOUT();
			break;
		case 501:	 
		case 506:	 
		case 508:	 
		case 512:	 
		case 514:	 
		case 517:	 
		case 518:	 
		case 519:	 
		case 530:	 
			ND_PRINT((ndo, " id"));
			INTOUT();
			break;
		case 502:	 
			ND_PRINT((ndo, " pos"));
			INTOUT();
			break;
		case 503:	 
		case 507:	 
		case 515:	 
			ND_PRINT((ndo, " uid"));
			INTOUT();
			ND_PRINT((ndo, " gid"));
			INTOUT();
			break;
		case 504:	 
		{
			unsigned long j;
			ND_TCHECK2(bp[0], 4);
			j = EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);

			 

			for (i = 0; i < j; i++) {
				VECOUT(PRNAMEMAX);
			}
			if (j == 0)
				ND_PRINT((ndo, " <none!>"));
		}
			break;
		case 505:	 
		{
			unsigned long j;
			ND_PRINT((ndo, " ids:"));
			ND_TCHECK2(bp[0], 4);
			i = EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);
			for (j = 0; j < i; j++)
				INTOUT();
			if (j == 0)
				ND_PRINT((ndo, " <none!>"));
		}
			break;
		case 509:	 
			STROUT(PRNAMEMAX);
			ND_PRINT((ndo, " flag"));
			INTOUT();
			ND_PRINT((ndo, " oid"));
			INTOUT();
			break;
		case 511:	 
			ND_PRINT((ndo, " id"));
			INTOUT();
			ND_PRINT((ndo, " gflag"));
			INTOUT();
			break;
		case 513:	 
			ND_PRINT((ndo, " id"));
			INTOUT();
			STROUT(PRNAMEMAX);
			ND_PRINT((ndo, " oldid"));
			INTOUT();
			ND_PRINT((ndo, " newid"));
			INTOUT();
			break;
		case 520:	 
			ND_PRINT((ndo, " id"));
			INTOUT();
			STROUT(PRNAMEMAX);
			break;
		default:
			;
	}


	return;

trunc:
	ND_PRINT((ndo, " [|pt]"));
}
