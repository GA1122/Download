vol_print(netdissect_options *ndo,
          register const u_char *bp, int length)
{
	int vol_op;

	if (length <= (int)sizeof(struct rx_header))
		return;

	if (ndo->ndo_snapend - bp + 1 <= (int)(sizeof(struct rx_header) + sizeof(int32_t))) {
		goto trunc;
	}

	 

	vol_op = EXTRACT_32BITS(bp + sizeof(struct rx_header));

	ND_PRINT((ndo, " vol call %s", tok2str(vol_req, "op#%d", vol_op)));

	bp += sizeof(struct rx_header) + 4;

	switch (vol_op) {
		case 100:	 
			ND_PRINT((ndo, " partition"));
			UINTOUT();
			ND_PRINT((ndo, " name"));
			STROUT(AFSNAMEMAX);
			ND_PRINT((ndo, " type"));
			UINTOUT();
			ND_PRINT((ndo, " parent"));
			UINTOUT();
			break;
		case 101:	 
		case 107:	 
			ND_PRINT((ndo, " trans"));
			UINTOUT();
			break;
		case 102:	 
			ND_PRINT((ndo, " totrans"));
			UINTOUT();
			ND_PRINT((ndo, " flags"));
			UINTOUT();
			break;
		case 103:	 
			ND_PRINT((ndo, " fromtrans"));
			UINTOUT();
			ND_PRINT((ndo, " fromdate"));
			DATEOUT();
			DESTSERVEROUT();
			ND_PRINT((ndo, " desttrans"));
			INTOUT();
			break;
		case 104:	 
			ND_PRINT((ndo, " trans"));
			UINTOUT();
			break;
		case 105:	 
			ND_PRINT((ndo, " trans"));
			UINTOUT();
			ND_PRINT((ndo, " purgevol"));
			UINTOUT();
			ND_PRINT((ndo, " newtype"));
			UINTOUT();
			ND_PRINT((ndo, " newname"));
			STROUT(AFSNAMEMAX);
			break;
		case 106:	 
			ND_PRINT((ndo, " trans"));
			UINTOUT();
			ND_PRINT((ndo, " flags"));
			UINTOUT();
			break;
		case 108:	 
			ND_PRINT((ndo, " vol"));
			UINTOUT();
			ND_PRINT((ndo, " partition"));
			UINTOUT();
			ND_PRINT((ndo, " flags"));
			UINTOUT();
			break;
		case 109:	 
		case 655537:	 
			ND_PRINT((ndo, " fromtrans"));
			UINTOUT();
			ND_PRINT((ndo, " fromdate"));
			DATEOUT();
			break;
		case 110:	 
			ND_PRINT((ndo, " index"));
			UINTOUT();
			break;
		case 111:	 
			ND_PRINT((ndo, " tid"));
			UINTOUT();
			ND_PRINT((ndo, " newsite"));
			UINTOUT();
			break;
		case 112:	 
		case 113:	 
			ND_PRINT((ndo, " tid"));
			break;
		case 114:	 
			ND_PRINT((ndo, " name"));
			STROUT(AFSNAMEMAX);
			ND_PRINT((ndo, " type"));
			UINTOUT();
			ND_PRINT((ndo, " pid"));
			UINTOUT();
			ND_PRINT((ndo, " cloneid"));
			UINTOUT();
			break;
		case 116:	 
			ND_PRINT((ndo, " partition"));
			UINTOUT();
			ND_PRINT((ndo, " flags"));
			UINTOUT();
			break;
		case 117:	 
			ND_PRINT((ndo, " tid"));
			UINTOUT();
			ND_PRINT((ndo, " name"));
			STROUT(AFSNAMEMAX);
			ND_PRINT((ndo, " type"));
			UINTOUT();
			ND_PRINT((ndo, " pid"));
			UINTOUT();
			ND_PRINT((ndo, " clone"));
			UINTOUT();
			ND_PRINT((ndo, " backup"));
			UINTOUT();
			break;
		case 119:	 
			ND_PRINT((ndo, " name"));
			STROUT(AFSNAMEMAX);
			break;
		case 120:	 
			ND_PRINT((ndo, " tid"));
			UINTOUT();
			break;
		case 121:	 
		case 122:	 
		case 124:	 
		case 125:	 
		case 65536:	 
			ND_PRINT((ndo, " partid"));
			UINTOUT();
			ND_PRINT((ndo, " volid"));
			UINTOUT();
			break;
		case 123:	 
			ND_PRINT((ndo, " tid"));
			UINTOUT();
			ND_PRINT((ndo, " date"));
			DATEOUT();
			break;
		case 126:	 
			ND_PRINT((ndo, " tid"));
			UINTOUT();
			break;
		case 128:	 
			ND_PRINT((ndo, " fromtrans"));
			UINTOUT();
			ND_PRINT((ndo, " fromdate"));
			DATEOUT();
			{
				unsigned long i, j;
				ND_TCHECK2(bp[0], 4);
				j = EXTRACT_32BITS(bp);
				bp += sizeof(int32_t);
				for (i = 0; i < j; i++) {
					DESTSERVEROUT();
					if (i != j - 1)
						ND_PRINT((ndo, ","));
				}
				if (j == 0)
					ND_PRINT((ndo, " <none!>"));
			}
			break;
		case 65538:	 
			ND_PRINT((ndo, " fromtrans"));
			UINTOUT();
			ND_PRINT((ndo, " fromdate"));
			DATEOUT();
			ND_PRINT((ndo, " flags"));
			UINTOUT();
			break;
		default:
			;
	}
	return;

trunc:
	ND_PRINT((ndo, " [|vol]"));
}
