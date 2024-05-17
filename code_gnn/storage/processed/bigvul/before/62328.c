ppp_hdlc(netdissect_options *ndo,
         const u_char *p, int length)
{
	u_char *b, *t, c;
	const u_char *s;
	int i, proto;
	const void *se;

        if (length <= 0)
                return;

	b = (u_char *)malloc(length);
	if (b == NULL)
		return;

	 
	for (s = p, t = b, i = length; i > 0 && ND_TTEST(*s); i--) {
		c = *s++;
		if (c == 0x7d) {
			if (i <= 1 || !ND_TTEST(*s))
				break;
			i--;
			c = *s++ ^ 0x20;
		}
		*t++ = c;
	}

	se = ndo->ndo_snapend;
	ndo->ndo_snapend = t;
	length = t - b;

         
        if (length < 1)
                goto trunc;
        proto = *b;  

        switch (proto) {
        case PPP_IP:
		ip_print(ndo, b + 1, length - 1);
		goto cleanup;
        case PPP_IPV6:
		ip6_print(ndo, b + 1, length - 1);
		goto cleanup;
        default:  
		break;
        }

        if (length < 2)
                goto trunc;
        proto = EXTRACT_16BITS(b);  

        switch (proto) {
        case (PPP_ADDRESS << 8 | PPP_CONTROL):  
            if (length < 4)
                goto trunc;
            proto = EXTRACT_16BITS(b+2);  
            handle_ppp(ndo, proto, b + 4, length - 4);
            break;
        default:  
            handle_ppp(ndo, proto, b + 2, length - 2);
            break;
        }

cleanup:
	ndo->ndo_snapend = se;
	free(b);
        return;

trunc:
	ndo->ndo_snapend = se;
	free(b);
	ND_PRINT((ndo, "[|ppp]"));
}