ppp_if_print(netdissect_options *ndo,
             const struct pcap_pkthdr *h, register const u_char *p)
{
	register u_int length = h->len;
	register u_int caplen = h->caplen;

	if (caplen < PPP_HDRLEN) {
		ND_PRINT((ndo, "[|ppp]"));
		return (caplen);
	}

#if 0
	 
	if (ndo->ndo_eflag)
		ND_PRINT((ndo, "%c %4d %02x ", p[0] ? 'O' : 'I', length, p[1]));
#endif

	ppp_print(ndo, p, length);

	return (0);
}
