archive_read_format_cpio_bid(struct archive_read *a, int best_bid)
{
	const unsigned char *p;
	struct cpio *cpio;
	int bid;

	(void)best_bid;  

	cpio = (struct cpio *)(a->format->data);

	if ((p = __archive_read_ahead(a, 6, NULL)) == NULL)
		return (-1);

	bid = 0;
	if (memcmp(p, "070707", 6) == 0) {
		 
		cpio->read_header = header_odc;
		bid += 48;
		 
	} else if (memcmp(p, "070727", 6) == 0) {
		 
		cpio->read_header = header_odc;
		bid += 48;
		 
	} else if (memcmp(p, "070701", 6) == 0) {
		 
		cpio->read_header = header_newc;
		bid += 48;
		 
	} else if (memcmp(p, "070702", 6) == 0) {
		 
		 
		cpio->read_header = header_newc;
		bid += 48;
		 
	} else if (p[0] * 256 + p[1] == 070707) {
		 
		cpio->read_header = header_bin_be;
		bid += 16;
		 
	} else if (p[0] + p[1] * 256 == 070707) {
		 
		cpio->read_header = header_bin_le;
		bid += 16;
		 
	} else
		return (ARCHIVE_WARN);

	return (bid);
}
