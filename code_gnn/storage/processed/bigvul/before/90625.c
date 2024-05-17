archive_read_format_zip_seekable_bid(struct archive_read *a, int best_bid)
{
	struct zip *zip = (struct zip *)a->format->data;
	int64_t file_size, current_offset;
	const char *p;
	int i, tail;

	 
	if (best_bid > 32)
		return (-1);

	file_size = __archive_read_seek(a, 0, SEEK_END);
	if (file_size <= 0)
		return 0;

	 
	tail = (int)zipmin(1024 * 16, file_size);
	current_offset = __archive_read_seek(a, -tail, SEEK_END);
	if (current_offset < 0)
		return 0;
	if ((p = __archive_read_ahead(a, (size_t)tail, NULL)) == NULL)
		return 0;
	 
	for (i = tail - 22; i > 0;) {
		switch (p[i]) {
		case 'P':
			if (memcmp(p + i, "PK\005\006", 4) == 0) {
				int ret = read_eocd(zip, p + i,
				    current_offset + i);
				 
				if (i >= 20 && memcmp(p + i - 20, "PK\006\007", 4) == 0) {
					int ret_zip64 = read_zip64_eocd(a, zip, p + i - 20);
					if (ret_zip64 > ret)
						ret = ret_zip64;
				}
				return (ret);
			}
			i -= 4;
			break;
		case 'K': i -= 1; break;
		case 005: i -= 2; break;
		case 006: i -= 3; break;
		default: i -= 4; break;
		}
	}
	return 0;
}
