dnsname_print(netdissect_options *ndo, const u_char *cp, const u_char *ep)
{
	int i;

	 
	ND_PRINT((ndo,", \""));
	while (cp < ep) {
		i = *cp++;
		if (i) {
			if (i > ep - cp) {
				ND_PRINT((ndo,"???"));
				break;
			}
			while (i-- && cp < ep) {
				safeputchar(ndo, *cp);
				cp++;
			}
			if (cp + 1 < ep && *cp)
				ND_PRINT((ndo,"."));
		} else {
			if (cp == ep) {
				 
				ND_PRINT((ndo,"."));
			} else if (cp + 1 == ep && *cp == '\0') {
				 
			} else {
				 
				ND_PRINT((ndo,"???"));
			}
			break;
		}
	}
	ND_PRINT((ndo,"\""));
}
