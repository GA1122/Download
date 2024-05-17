sig_print(netdissect_options *ndo,
          const u_char *p)
{
	uint32_t call_ref;

	ND_TCHECK(p[PROTO_POS]);
	if (p[PROTO_POS] == Q2931) {
		 
		ND_PRINT((ndo, "Q.2931"));
		ND_TCHECK(p[MSG_TYPE_POS]);
		ND_PRINT((ndo, ":%s ",
		    tok2str(msgtype2str, "msgtype#%d", p[MSG_TYPE_POS])));

		 
		call_ref = EXTRACT_24BITS(&p[CALL_REF_POS]);
		ND_PRINT((ndo, "CALL_REF:0x%06x", call_ref));
	} else {
		 
		ND_PRINT((ndo, "SSCOP, proto %d ", p[PROTO_POS]));
	}
	return;

trunc:
	ND_PRINT((ndo, " %s", tstr));
}
