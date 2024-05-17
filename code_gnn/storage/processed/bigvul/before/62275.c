kauth_print(netdissect_options *ndo,
            register const u_char *bp, int length)
{
	int kauth_op;

	if (length <= (int)sizeof(struct rx_header))
		return;

	if (ndo->ndo_snapend - bp + 1 <= (int)(sizeof(struct rx_header) + sizeof(int32_t))) {
		goto trunc;
	}

	 

	kauth_op = EXTRACT_32BITS(bp + sizeof(struct rx_header));

	ND_PRINT((ndo, " kauth"));

	if (is_ubik(kauth_op)) {
		ubik_print(ndo, bp);
		return;
	}


	ND_PRINT((ndo, " call %s", tok2str(kauth_req, "op#%d", kauth_op)));

	 

	bp += sizeof(struct rx_header) + 4;

	switch (kauth_op) {
		case 1:		 
		case 21:	 
		case 22:	 
		case 2:		 
		case 5:		 
		case 6:		 
		case 7:		 
		case 8:		 
		case 14:	 
		case 15:	 
			ND_PRINT((ndo, " principal"));
			STROUT(KANAMEMAX);
			STROUT(KANAMEMAX);
			break;
		case 3:		 
		case 23:	 
		{
			int i;
			ND_PRINT((ndo, " kvno"));
			INTOUT();
			ND_PRINT((ndo, " domain"));
			STROUT(KANAMEMAX);
			ND_TCHECK2(bp[0], sizeof(int32_t));
			i = (int) EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);
			ND_TCHECK2(bp[0], i);
			bp += i;
			ND_PRINT((ndo, " principal"));
			STROUT(KANAMEMAX);
			STROUT(KANAMEMAX);
			break;
		}
		case 4:		 
			ND_PRINT((ndo, " principal"));
			STROUT(KANAMEMAX);
			STROUT(KANAMEMAX);
			ND_PRINT((ndo, " kvno"));
			INTOUT();
			break;
		case 12:	 
			ND_PRINT((ndo, " name"));
			STROUT(KANAMEMAX);
			break;
		default:
			;
	}

	return;

trunc:
	ND_PRINT((ndo, " [|kauth]"));
}
