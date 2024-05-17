ike_sub0_print(netdissect_options *ndo,
		 u_char np, const struct isakmp_gen *ext, const u_char *ep,

	       uint32_t phase, uint32_t doi, uint32_t proto, int depth)
{
	const u_char *cp;
	struct isakmp_gen e;
	u_int item_len;

	cp = (const u_char *)ext;
	ND_TCHECK(*ext);
	UNALIGNED_MEMCPY(&e, ext, sizeof(e));

	 
	item_len = ntohs(e.len);
	if (item_len <= 4)
		return NULL;

	if (NPFUNC(np)) {
		 
		cp = (*npfunc[np])(ndo, np, ext, item_len, ep, phase, doi, proto, depth);
	} else {
		ND_PRINT((ndo,"%s", NPSTR(np)));
		cp += item_len;
	}

	return cp;
trunc:
	ND_PRINT((ndo," [|isakmp]"));
	return NULL;
}
