prot_reply_print(netdissect_options *ndo,
                 register const u_char *bp, int length, int32_t opcode)
{
	const struct rx_header *rxh;
	unsigned long i;

	if (length < (int)sizeof(struct rx_header))
		return;

	rxh = (const struct rx_header *) bp;

	 

	ND_PRINT((ndo, " pt"));

	if (is_ubik(opcode)) {
		ubik_reply_print(ndo, bp, length, opcode);
		return;
	}

	ND_PRINT((ndo, " reply %s", tok2str(pt_req, "op#%d", opcode)));

	bp += sizeof(struct rx_header);

	 

	if (rxh->type == RX_PACKET_TYPE_DATA)
		switch (opcode) {
		case 504:		 
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
		case 505:		 
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
		case 508:		 
		case 514:		 
		case 517:		 
		case 518:		 
		case 519:		 
		{
			unsigned long j;
			ND_TCHECK2(bp[0], 4);
			j = EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);
			for (i = 0; i < j; i++) {
				INTOUT();
			}
			if (j == 0)
				ND_PRINT((ndo, " <none!>"));
		}
			break;
		case 510:		 
			ND_PRINT((ndo, " maxuid"));
			INTOUT();
			ND_PRINT((ndo, " maxgid"));
			INTOUT();
			break;
		default:
			;
		}
	else {
		 
		ND_PRINT((ndo, " errcode"));
		INTOUT();
	}

	return;

trunc:
	ND_PRINT((ndo, " [|pt]"));
}