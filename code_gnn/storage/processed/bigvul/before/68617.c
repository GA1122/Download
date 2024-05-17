archive_read_format_lha_bid(struct archive_read *a, int best_bid)
{
	const char *p;
	const void *buff;
	ssize_t bytes_avail, offset, window;
	size_t next;

	 
	if (best_bid > 30)
		return (-1);

	if ((p = __archive_read_ahead(a, H_SIZE, NULL)) == NULL)
		return (-1);

	if (lha_check_header_format(p) == 0)
		return (30);

	if (p[0] == 'M' && p[1] == 'Z') {
		 
		offset = 0;
		window = 4096;
		while (offset < (1024 * 20)) {
			buff = __archive_read_ahead(a, offset + window,
			    &bytes_avail);
			if (buff == NULL) {
				 
				window >>= 1;
				if (window < (H_SIZE + 3))
					return (0);
				continue;
			}
			p = (const char *)buff + offset;
			while (p + H_SIZE < (const char *)buff + bytes_avail) {
				if ((next = lha_check_header_format(p)) == 0)
					return (30);
				p += next;
			}
			offset = p - (const char *)buff;
		}
	}
	return (0);
}
