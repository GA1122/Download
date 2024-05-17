rx_ack_print(netdissect_options *ndo,
             register const u_char *bp, int length)
{
	const struct rx_ackPacket *rxa;
	int i, start, last;
	uint32_t firstPacket;

	if (length < (int)sizeof(struct rx_header))
		return;

	bp += sizeof(struct rx_header);

	 

	ND_TCHECK2(bp[0], sizeof(struct rx_ackPacket) - RX_MAXACKS);

	rxa = (const struct rx_ackPacket *) bp;
	bp += (sizeof(struct rx_ackPacket) - RX_MAXACKS);

	 

	if (ndo->ndo_vflag > 2)
		ND_PRINT((ndo, " bufspace %d maxskew %d",
		       (int) EXTRACT_16BITS(&rxa->bufferSpace),
		       (int) EXTRACT_16BITS(&rxa->maxSkew)));

	firstPacket = EXTRACT_32BITS(&rxa->firstPacket);
	ND_PRINT((ndo, " first %d serial %d reason %s",
	       firstPacket, EXTRACT_32BITS(&rxa->serial),
	       tok2str(rx_ack_reasons, "#%d", (int) rxa->reason)));

	 

	if (rxa->nAcks != 0) {

		ND_TCHECK2(bp[0], rxa->nAcks);

		 

		for (i = 0, start = last = -2; i < rxa->nAcks; i++)
			if (rxa->acks[i] == RX_ACK_TYPE_ACK) {

				 

				if (last == -2) {
					ND_PRINT((ndo, " acked %d", firstPacket + i));
					start = i;
				}

				 

				else if (last != i - 1) {
					ND_PRINT((ndo, ",%d", firstPacket + i));
					start = i;
				}

				 

				last = i;

				 
			} else if (last == i - 1 && start != last)
				ND_PRINT((ndo, "-%d", firstPacket + i - 1));

		 

		if (last == i - 1 && start != last)
			ND_PRINT((ndo, "-%d", firstPacket + i - 1));

		 

		for (i = 0, start = last = -2; i < rxa->nAcks; i++)
			if (rxa->acks[i] == RX_ACK_TYPE_NACK) {
				if (last == -2) {
					ND_PRINT((ndo, " nacked %d", firstPacket + i));
					start = i;
				} else if (last != i - 1) {
					ND_PRINT((ndo, ",%d", firstPacket + i));
					start = i;
				}
				last = i;
			} else if (last == i - 1 && start != last)
				ND_PRINT((ndo, "-%d", firstPacket + i - 1));

		if (last == i - 1 && start != last)
			ND_PRINT((ndo, "-%d", firstPacket + i - 1));

		bp += rxa->nAcks;
	}


	 

#define TRUNCRET(n)	if (ndo->ndo_snapend - bp + 1 <= n) return;

	if (ndo->ndo_vflag > 1) {
		TRUNCRET(4);
		ND_PRINT((ndo, " ifmtu"));
		INTOUT();

		TRUNCRET(4);
		ND_PRINT((ndo, " maxmtu"));
		INTOUT();

		TRUNCRET(4);
		ND_PRINT((ndo, " rwind"));
		INTOUT();

		TRUNCRET(4);
		ND_PRINT((ndo, " maxpackets"));
		INTOUT();
	}

	return;

trunc:
	ND_PRINT((ndo, " [|ack]"));
}
