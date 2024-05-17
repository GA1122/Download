gpk_parse_fci(sc_card_t *card,
		const u8 *buf, size_t buflen,
		sc_file_t *file)
{
	const u8	*end, *next;
	unsigned int	tag, len;

	end = buf + buflen;
	for (; buf + 2 < end; buf = next) {
		next = buf + 2 + buf[1];
		if (next > end)
			break;
		tag = *buf++;
		len = *buf++;
		if (tag == 0x84) {
			 
		} else
		if (tag == 0xC1 && len >= 2) {
			 
			file->id = (buf[0] << 8) | buf[1];
		} else
		if (tag == 0xC2) {
			 
		}
	}

	return 0;
}
