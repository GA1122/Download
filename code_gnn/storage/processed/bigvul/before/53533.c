archive_read_format_7zip_bid(struct archive_read *a, int best_bid)
{
	const char *p;

	 
	if (best_bid > 32)
		return (-1);

	if ((p = __archive_read_ahead(a, 6, NULL)) == NULL)
		return (0);

	 
	if (memcmp(p, _7ZIP_SIGNATURE, 6) == 0)
		return (48);

	 
	if ((p[0] == 'M' && p[1] == 'Z') || memcmp(p, "\x7F\x45LF", 4) == 0) {
		ssize_t offset = SFX_MIN_ADDR;
		ssize_t window = 4096;
		ssize_t bytes_avail;
		while (offset + window <= (SFX_MAX_ADDR)) {
			const char *buff = __archive_read_ahead(a,
					offset + window, &bytes_avail);
			if (buff == NULL) {
				 
				window >>= 1;
				if (window < 0x40)
					return (0);
				continue;
			}
			p = buff + offset;
			while (p + 32 < buff + bytes_avail) {
				int step = check_7zip_header_in_sfx(p);
				if (step == 0)
					return (48);
				p += step;
			}
			offset = p - buff;
		}
	}
	return (0);
}
