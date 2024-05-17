ikev2_print(netdissect_options *ndo,
	    const u_char *bp,  u_int length,
	    const u_char *bp2 _U_, struct isakmp *base)
{
	const struct isakmp *p;
	const u_char *ep;
	u_char np;
	int phase;

	p = (const struct isakmp *)bp;
	ep = ndo->ndo_snapend;

	phase = (EXTRACT_32BITS(base->msgid) == 0) ? 1 : 2;
	if (phase == 1)
		ND_PRINT((ndo, " parent_sa"));
	else
		ND_PRINT((ndo, " child_sa "));

	ND_PRINT((ndo, " %s", ETYPESTR(base->etype)));
	if (base->flags) {
		ND_PRINT((ndo, "[%s%s%s]",
			  base->flags & ISAKMP_FLAG_I ? "I" : "",
			  base->flags & ISAKMP_FLAG_V ? "V" : "",
			  base->flags & ISAKMP_FLAG_R ? "R" : ""));
	}

	if (ndo->ndo_vflag) {
		const struct isakmp_gen *ext;

		ND_PRINT((ndo, ":"));

		 
		if (base->flags & ISAKMP_FLAG_E) {
			 
			ND_PRINT((ndo, " [encrypted %s]", NPSTR(base->np)));
			goto done;
		}

		CHECKLEN(p + 1, base->np)

		np = base->np;
		ext = (const struct isakmp_gen *)(p + 1);
		ikev2_sub_print(ndo, base, np, ext, ep, phase, 0, 0, 0);
	}

done:
	if (ndo->ndo_vflag) {
		if (ntohl(base->len) != length) {
			ND_PRINT((ndo, " (len mismatch: isakmp %u/ip %u)",
				  (uint32_t)ntohl(base->len), length));
		}
	}
}