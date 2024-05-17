netanalyzer_transparent_if_print(netdissect_options *ndo,
                                 const struct pcap_pkthdr *h,
                                 const u_char *p)
{
	 
	if (h->len < 12 || h->caplen < 12) {
		ND_PRINT((ndo, "[|netanalyzer-transparent]"));
		return (h->caplen);
	}

	 
	return (12 + ether_print(ndo, p + 12, h->len - 12, h->caplen - 12, NULL, NULL));
}
