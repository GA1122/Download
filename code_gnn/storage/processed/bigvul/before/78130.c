const u8 *sc_asn1_find_tag(sc_context_t *ctx, const u8 * buf,
	size_t buflen, unsigned int tag_in, size_t *taglen_in)
{
	size_t left = buflen, taglen;
	const u8 *p = buf;

	*taglen_in = 0;
	while (left >= 2) {
		unsigned int cla = 0, tag, mask = 0xff00;

		buf = p;
		 
		if (sc_asn1_read_tag(&p, left, &cla, &tag, &taglen) != SC_SUCCESS
				|| p == NULL)
			return NULL;

		left -= (p - buf);
		 
		while ((tag & mask) != 0) {
			cla  <<= 8;
			mask <<= 8;
		}
		 
		if ((tag | cla) == tag_in) {
			 
			if (taglen > left)
				return NULL;
			*taglen_in = taglen;
			return p;
		}
		 
		left -= taglen;
		p += taglen;
	}
	return NULL;
}