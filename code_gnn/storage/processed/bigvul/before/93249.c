fs_print(netdissect_options *ndo,
         register const u_char *bp, int length)
{
	int fs_op;
	unsigned long i;

	if (length <= (int)sizeof(struct rx_header))
		return;

	if (ndo->ndo_snapend - bp + 1 <= (int)(sizeof(struct rx_header) + sizeof(int32_t))) {
		goto trunc;
	}

	 

	fs_op = EXTRACT_32BITS(bp + sizeof(struct rx_header));

	ND_PRINT((ndo, " fs call %s", tok2str(fs_req, "op#%d", fs_op)));

	 

	bp += sizeof(struct rx_header) + 4;

	 

	switch (fs_op) {
		case 130:	 
			FIDOUT();
			ND_PRINT((ndo, " offset"));
			UINTOUT();
			ND_PRINT((ndo, " length"));
			UINTOUT();
			break;
		case 131:	 
		case 132:	 
		case 143:	 
		case 144:	 
		case 145:	 
		case 156:	 
		case 157:	 
		case 158:	 
			FIDOUT();
			break;
		case 135:	 
			FIDOUT();
			STOREATTROUT();
			break;
		case 133:	 
			FIDOUT();
			STOREATTROUT();
			ND_PRINT((ndo, " offset"));
			UINTOUT();
			ND_PRINT((ndo, " length"));
			UINTOUT();
			ND_PRINT((ndo, " flen"));
			UINTOUT();
			break;
		case 134:	 
		{
			char a[AFSOPAQUEMAX+1];
			FIDOUT();
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
			FIDOUT();
			STROUT(AFSNAMEMAX);
			STOREATTROUT();
			break;
		case 136:	 
		case 142:	 
			FIDOUT();
			STROUT(AFSNAMEMAX);
			break;
		case 138:	 
			ND_PRINT((ndo, " old"));
			FIDOUT();
			STROUT(AFSNAMEMAX);
			ND_PRINT((ndo, " new"));
			FIDOUT();
			STROUT(AFSNAMEMAX);
			break;
		case 139:	 
			FIDOUT();
			STROUT(AFSNAMEMAX);
			ND_PRINT((ndo, " link to"));
			STROUT(AFSNAMEMAX);
			break;
		case 140:	 
			FIDOUT();
			STROUT(AFSNAMEMAX);
			ND_PRINT((ndo, " link to"));
			FIDOUT();
			break;
		case 148:	 
			STROUT(AFSNAMEMAX);
			break;
		case 149:	 
		case 150:	 
			ND_PRINT((ndo, " volid"));
			UINTOUT();
			break;
		case 154:	 
			ND_PRINT((ndo, " volname"));
			STROUT(AFSNAMEMAX);
			break;
		case 155:	 
		case 65536:      
		{
			unsigned long j;
			ND_TCHECK2(bp[0], 4);
			j = EXTRACT_32BITS(bp);
			bp += sizeof(int32_t);

			for (i = 0; i < j; i++) {
				FIDOUT();
				if (i != j - 1)
					ND_PRINT((ndo, ","));
			}
			if (j == 0)
				ND_PRINT((ndo, " <none!>"));
			break;
		}
		case 65537:	 
			FIDOUT();
			ND_PRINT((ndo, " offset"));
			UINT64OUT();
			ND_PRINT((ndo, " length"));
			UINT64OUT();
			break;
		case 65538:	 
			FIDOUT();
			STOREATTROUT();
			ND_PRINT((ndo, " offset"));
			UINT64OUT();
			ND_PRINT((ndo, " length"));
			UINT64OUT();
			ND_PRINT((ndo, " flen"));
			UINT64OUT();
			break;
		case 65541:     
			ND_PRINT((ndo, " addr"));
			UINTOUT();
		default:
			;
	}

	return;

trunc:
	ND_PRINT((ndo, " [|fs]"));
}
