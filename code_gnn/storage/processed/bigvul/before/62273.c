fs_reply_print(netdissect_options *ndo,
               register const u_char *bp, int length, int32_t opcode)
{
	unsigned long i;
	const struct rx_header *rxh;

	if (length <= (int)sizeof(struct rx_header))
		return;

	rxh = (const struct rx_header *) bp;

	 

	ND_PRINT((ndo, " fs reply %s", tok2str(fs_req, "op#%d", opcode)));

	bp += sizeof(struct rx_header);

	 

	if (rxh->type == RX_PACKET_TYPE_DATA) {
		switch (opcode) {
		case 131:	 
		{
			char a[AFSOPAQUEMAX+1];
			ND_TCHECK2(bp[0], 4);
			i = EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);
			ND_TCHECK2(bp[0], i);
			i = min(AFSOPAQUEMAX, i);
			strncpy(a, (const char *) bp, i);
			a[i] = '\0';
			acl_print(ndo, (u_char *) a, sizeof(a), (u_char *) a + i);
			break;
		}
		case 137:	 
		case 141:	 
			ND_PRINT((ndo, " new"));
			FIDOUT();
			break;
		case 151:	 
			ND_PRINT((ndo, " root volume"));
			STROUT(AFSNAMEMAX);
			break;
		case 153:	 
			DATEOUT();
			break;
		default:
			;
		}
	} else if (rxh->type == RX_PACKET_TYPE_ABORT) {
		 
		ND_TCHECK2(bp[0], sizeof(int32_t));
		i = (int) EXTRACT_32BITS(bp);
		bp += sizeof(int32_t);

		ND_PRINT((ndo, " error %s", tok2str(afs_fs_errors, "#%d", i)));
	} else {
		ND_PRINT((ndo, " strange fs reply of type %d", rxh->type));
	}

	return;

trunc:
	ND_PRINT((ndo, " [|fs]"));
}
