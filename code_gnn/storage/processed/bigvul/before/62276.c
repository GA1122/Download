kauth_reply_print(netdissect_options *ndo,
                  register const u_char *bp, int length, int32_t opcode)
{
	const struct rx_header *rxh;

	if (length <= (int)sizeof(struct rx_header))
		return;

	rxh = (const struct rx_header *) bp;

	 

	ND_PRINT((ndo, " kauth"));

	if (is_ubik(opcode)) {
		ubik_reply_print(ndo, bp, length, opcode);
		return;
	}

	ND_PRINT((ndo, " reply %s", tok2str(kauth_req, "op#%d", opcode)));

	bp += sizeof(struct rx_header);

	 

	if (rxh->type == RX_PACKET_TYPE_DATA)
		 
		;
	else {
		 
		ND_PRINT((ndo, " errcode"));
		INTOUT();
	}

	return;

trunc:
	ND_PRINT((ndo, " [|kauth]"));
}
