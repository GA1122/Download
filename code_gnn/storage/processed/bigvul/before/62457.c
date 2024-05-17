parsefn(netdissect_options *ndo,
        register const uint32_t *dp)
{
	register uint32_t len;
	register const u_char *cp;

	 
	ND_TCHECK(*dp);

	 
	len = *dp++;
	NTOHL(len);

	ND_TCHECK2(*dp, ((len + 3) & ~3));

	cp = (const u_char *)dp;
	 
	dp += ((len + 3) & ~3) / sizeof(*dp);
	ND_PRINT((ndo, "\""));
	if (fn_printn(ndo, cp, len, ndo->ndo_snapend)) {
		ND_PRINT((ndo, "\""));
		goto trunc;
	}
	ND_PRINT((ndo, "\""));

	return (dp);
trunc:
	return NULL;
}
