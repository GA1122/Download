rx_print(netdissect_options *ndo,
         register const u_char *bp, int length, int sport, int dport,
         const u_char *bp2)
{
	register const struct rx_header *rxh;
	int i;
	int32_t opcode;

	if (ndo->ndo_snapend - bp < (int)sizeof (struct rx_header)) {
		ND_PRINT((ndo, " [|rx] (%d)", length));
		return;
	}

	rxh = (const struct rx_header *) bp;

	ND_PRINT((ndo, " rx %s", tok2str(rx_types, "type %d", rxh->type)));

	if (ndo->ndo_vflag) {
		int firstflag = 0;

		if (ndo->ndo_vflag > 1)
			ND_PRINT((ndo, " cid %08x call# %d",
			       (int) EXTRACT_32BITS(&rxh->cid),
			       (int) EXTRACT_32BITS(&rxh->callNumber)));

		ND_PRINT((ndo, " seq %d ser %d",
		       (int) EXTRACT_32BITS(&rxh->seq),
		       (int) EXTRACT_32BITS(&rxh->serial)));

		if (ndo->ndo_vflag > 2)
			ND_PRINT((ndo, " secindex %d serviceid %hu",
				(int) rxh->securityIndex,
				EXTRACT_16BITS(&rxh->serviceId)));

		if (ndo->ndo_vflag > 1)
			for (i = 0; i < NUM_RX_FLAGS; i++) {
				if (rxh->flags & rx_flags[i].flag &&
				    (!rx_flags[i].packetType ||
				     rxh->type == rx_flags[i].packetType)) {
					if (!firstflag) {
						firstflag = 1;
						ND_PRINT((ndo, " "));
					} else {
						ND_PRINT((ndo, ","));
					}
					ND_PRINT((ndo, "<%s>", rx_flags[i].s));
				}
			}
	}

	 

	if (rxh->type == RX_PACKET_TYPE_DATA &&
	    EXTRACT_32BITS(&rxh->seq) == 1 &&
	    rxh->flags & RX_CLIENT_INITIATED) {

		 

		rx_cache_insert(ndo, bp, (const struct ip *) bp2, dport);

		switch (dport) {
			case FS_RX_PORT:	 
				fs_print(ndo, bp, length);
				break;
			case CB_RX_PORT:	 
				cb_print(ndo, bp, length);
				break;
			case PROT_RX_PORT:	 
				prot_print(ndo, bp, length);
				break;
			case VLDB_RX_PORT:	 
				vldb_print(ndo, bp, length);
				break;
			case KAUTH_RX_PORT:	 
				kauth_print(ndo, bp, length);
				break;
			case VOL_RX_PORT:	 
				vol_print(ndo, bp, length);
				break;
			case BOS_RX_PORT:	 
				bos_print(ndo, bp, length);
				break;
			default:
				;
		}

	 

	} else if (((rxh->type == RX_PACKET_TYPE_DATA &&
					EXTRACT_32BITS(&rxh->seq) == 1) ||
		    rxh->type == RX_PACKET_TYPE_ABORT) &&
		   (rxh->flags & RX_CLIENT_INITIATED) == 0 &&
		   rx_cache_find(rxh, (const struct ip *) bp2,
				 sport, &opcode)) {

		switch (sport) {
			case FS_RX_PORT:	 
				fs_reply_print(ndo, bp, length, opcode);
				break;
			case CB_RX_PORT:	 
				cb_reply_print(ndo, bp, length, opcode);
				break;
			case PROT_RX_PORT:	 
				prot_reply_print(ndo, bp, length, opcode);
				break;
			case VLDB_RX_PORT:	 
				vldb_reply_print(ndo, bp, length, opcode);
				break;
			case KAUTH_RX_PORT:	 
				kauth_reply_print(ndo, bp, length, opcode);
				break;
			case VOL_RX_PORT:	 
				vol_reply_print(ndo, bp, length, opcode);
				break;
			case BOS_RX_PORT:	 
				bos_reply_print(ndo, bp, length, opcode);
				break;
			default:
				;
		}

	 

	} else if (rxh->type == RX_PACKET_TYPE_ACK)
		rx_ack_print(ndo, bp, length);


	ND_PRINT((ndo, " (%d)", length));
}
