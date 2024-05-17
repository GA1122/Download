vldb_reply_print(netdissect_options *ndo,
                 register const u_char *bp, int length, int32_t opcode)
{
	const struct rx_header *rxh;
	unsigned long i;

	if (length < (int)sizeof(struct rx_header))
		return;

	rxh = (const struct rx_header *) bp;

	 

	ND_PRINT((ndo, " vldb"));

	if (is_ubik(opcode)) {
		ubik_reply_print(ndo, bp, length, opcode);
		return;
	}

	ND_PRINT((ndo, " reply %s", tok2str(vldb_req, "op#%d", opcode)));

	bp += sizeof(struct rx_header);

	 

	if (rxh->type == RX_PACKET_TYPE_DATA)
		switch (opcode) {
		case 510:	 
			ND_PRINT((ndo, " count"));
			INTOUT();
			ND_PRINT((ndo, " nextindex"));
			INTOUT();
		case 503:	 
		case 504:	 
		{	unsigned long nservers, j;
			VECOUT(VLNAMEMAX);
			ND_TCHECK2(bp[0], sizeof(int32_t));
			bp += sizeof(int32_t);
			ND_PRINT((ndo, " numservers"));
			ND_TCHECK2(bp[0], sizeof(int32_t));
			nservers = EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);
			ND_PRINT((ndo, " %lu", nservers));
			ND_PRINT((ndo, " servers"));
			for (i = 0; i < 8; i++) {
				ND_TCHECK2(bp[0], sizeof(int32_t));
				if (i < nservers)
					ND_PRINT((ndo, " %s",
					   intoa(((const struct in_addr *) bp)->s_addr)));
				bp += sizeof(int32_t);
			}
			ND_PRINT((ndo, " partitions"));
			for (i = 0; i < 8; i++) {
				ND_TCHECK2(bp[0], sizeof(int32_t));
				j = EXTRACT_32BITS(bp);
				if (i < nservers && j <= 26)
					ND_PRINT((ndo, " %c", 'a' + (int)j));
				else if (i < nservers)
					ND_PRINT((ndo, " %lu", j));
				bp += sizeof(int32_t);
			}
			ND_TCHECK2(bp[0], 8 * sizeof(int32_t));
			bp += 8 * sizeof(int32_t);
			ND_PRINT((ndo, " rwvol"));
			UINTOUT();
			ND_PRINT((ndo, " rovol"));
			UINTOUT();
			ND_PRINT((ndo, " backup"));
			UINTOUT();
		}
			break;
		case 505:	 
			ND_PRINT((ndo, " newvol"));
			UINTOUT();
			break;
		case 521:	 
		case 529:	 
			ND_PRINT((ndo, " count"));
			INTOUT();
			ND_PRINT((ndo, " nextindex"));
			INTOUT();
		case 518:	 
		case 519:	 
		{	unsigned long nservers, j;
			VECOUT(VLNAMEMAX);
			ND_PRINT((ndo, " numservers"));
			ND_TCHECK2(bp[0], sizeof(int32_t));
			nservers = EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);
			ND_PRINT((ndo, " %lu", nservers));
			ND_PRINT((ndo, " servers"));
			for (i = 0; i < 13; i++) {
				ND_TCHECK2(bp[0], sizeof(int32_t));
				if (i < nservers)
					ND_PRINT((ndo, " %s",
					   intoa(((const struct in_addr *) bp)->s_addr)));
				bp += sizeof(int32_t);
			}
			ND_PRINT((ndo, " partitions"));
			for (i = 0; i < 13; i++) {
				ND_TCHECK2(bp[0], sizeof(int32_t));
				j = EXTRACT_32BITS(bp);
				if (i < nservers && j <= 26)
					ND_PRINT((ndo, " %c", 'a' + (int)j));
				else if (i < nservers)
					ND_PRINT((ndo, " %lu", j));
				bp += sizeof(int32_t);
			}
			ND_TCHECK2(bp[0], 13 * sizeof(int32_t));
			bp += 13 * sizeof(int32_t);
			ND_PRINT((ndo, " rwvol"));
			UINTOUT();
			ND_PRINT((ndo, " rovol"));
			UINTOUT();
			ND_PRINT((ndo, " backup"));
			UINTOUT();
		}
			break;
		case 526:	 
		case 527:	 
		{	unsigned long nservers, j;
			VECOUT(VLNAMEMAX);
			ND_PRINT((ndo, " numservers"));
			ND_TCHECK2(bp[0], sizeof(int32_t));
			nservers = EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);
			ND_PRINT((ndo, " %lu", nservers));
			ND_PRINT((ndo, " servers"));
			for (i = 0; i < 13; i++) {
				if (i < nservers) {
					ND_PRINT((ndo, " afsuuid"));
					AFSUUIDOUT();
				} else {
					ND_TCHECK2(bp[0], 44);
					bp += 44;
				}
			}
			ND_TCHECK2(bp[0], 4 * 13);
			bp += 4 * 13;
			ND_PRINT((ndo, " partitions"));
			for (i = 0; i < 13; i++) {
				ND_TCHECK2(bp[0], sizeof(int32_t));
				j = EXTRACT_32BITS(bp);
				if (i < nservers && j <= 26)
					ND_PRINT((ndo, " %c", 'a' + (int)j));
				else if (i < nservers)
					ND_PRINT((ndo, " %lu", j));
				bp += sizeof(int32_t);
			}
			ND_TCHECK2(bp[0], 13 * sizeof(int32_t));
			bp += 13 * sizeof(int32_t);
			ND_PRINT((ndo, " rwvol"));
			UINTOUT();
			ND_PRINT((ndo, " rovol"));
			UINTOUT();
			ND_PRINT((ndo, " backup"));
			UINTOUT();
		}
		default:
			;
		}

	else {
		 
		ND_PRINT((ndo, " errcode"));
		INTOUT();
	}

	return;

trunc:
	ND_PRINT((ndo, " [|vldb]"));
}
