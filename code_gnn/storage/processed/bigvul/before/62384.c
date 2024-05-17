fn_printztn(netdissect_options *ndo,
         register const u_char *s, register u_int n, register const u_char *ep)
{
	register u_int bytes;
	register u_char c;

	bytes = 0;
	for (;;) {
		if (n == 0 || (ep != NULL && s >= ep)) {
			 
			bytes = 0;
			break;
		}

		c = *s++;
		bytes++;
		n--;
		if (c == '\0') {
			 
			break;
		}
		if (!ND_ISASCII(c)) {
			c = ND_TOASCII(c);
			ND_PRINT((ndo, "M-"));
		}
		if (!ND_ISPRINT(c)) {
			c ^= 0x40;	 
			ND_PRINT((ndo, "^"));
		}
		ND_PRINT((ndo, "%c", c));
	}
	return(bytes);
}
