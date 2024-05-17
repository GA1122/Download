ubik_reply_print(netdissect_options *ndo,
                 register const u_char *bp, int length, int32_t opcode)
{
	const struct rx_header *rxh;

	if (length < (int)sizeof(struct rx_header))
		return;

	rxh = (const struct rx_header *) bp;

	 

	ND_PRINT((ndo, " ubik reply %s", tok2str(ubik_req, "op#%d", opcode)));

	bp += sizeof(struct rx_header);

	 

	if (rxh->type == RX_PACKET_TYPE_DATA)
		switch (opcode) {
		case 10000:		 
			ND_PRINT((ndo, " vote no"));
			break;
		case 20004:		 
			ND_PRINT((ndo, " dbversion"));
			UBIK_VERSIONOUT();
			break;
		default:
			;
		}

	 

	else
		switch (opcode) {
		case 10000:		 
			ND_PRINT((ndo, " vote yes until"));
			DATEOUT();
			break;
		default:
			ND_PRINT((ndo, " errcode"));
			INTOUT();
		}

	return;

trunc:
	ND_PRINT((ndo, " [|ubik]"));
}
