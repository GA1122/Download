_warc_bid(struct archive_read *a, int best_bid)
{
	const char *hdr;
	ssize_t nrd;
	unsigned int ver;

	(void)best_bid;  

	 
	if ((hdr = __archive_read_ahead(a, 12U, &nrd)) == NULL) {
		 
		return -1;
	} else if (nrd < 12) {
		 
		return -1;
	}

	 
	ver = _warc_rdver(hdr, nrd);
	if (ver < 1200U || ver > 10000U) {
		 
		return -1;
	}

	 
	return (64);
}
