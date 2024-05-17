bos_print(netdissect_options *ndo,
          register const u_char *bp, int length)
{
	int bos_op;

	if (length <= (int)sizeof(struct rx_header))
		return;

	if (ndo->ndo_snapend - bp + 1 <= (int)(sizeof(struct rx_header) + sizeof(int32_t))) {
		goto trunc;
	}

	 

	bos_op = EXTRACT_32BITS(bp + sizeof(struct rx_header));

	ND_PRINT((ndo, " bos call %s", tok2str(bos_req, "op#%d", bos_op)));

	 

	bp += sizeof(struct rx_header) + 4;

	switch (bos_op) {
		case 80:	 
			ND_PRINT((ndo, " type"));
			STROUT(BOSNAMEMAX);
			ND_PRINT((ndo, " instance"));
			STROUT(BOSNAMEMAX);
			break;
		case 81:	 
		case 83:	 
		case 85:	 
		case 87:	 
		case 88:	 
		case 93:	 
		case 96:	 
		case 97:	 
		case 104:	 
		case 106:	 
		case 108:	 
		case 112:	 
		case 114:	 
			STROUT(BOSNAMEMAX);
			break;
		case 82:	 
		case 98:	 
			STROUT(BOSNAMEMAX);
			ND_PRINT((ndo, " status"));
			INTOUT();
			break;
		case 86:	 
			STROUT(BOSNAMEMAX);
			ND_PRINT((ndo, " num"));
			INTOUT();
			break;
		case 84:	 
		case 89:	 
		case 90:	 
		case 91:	 
		case 92:	 
		case 95:	 
			INTOUT();
			break;
		case 105:	 
			STROUT(BOSNAMEMAX);
			ND_PRINT((ndo, " size"));
			INTOUT();
			ND_PRINT((ndo, " flags"));
			INTOUT();
			ND_PRINT((ndo, " date"));
			INTOUT();
			break;
		default:
			;
	}

	return;

trunc:
	ND_PRINT((ndo, " [|bos]"));
}
