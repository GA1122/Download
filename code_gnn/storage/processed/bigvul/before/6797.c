int del_hdr_value(struct buffer *buf, char **from, char *next)
{
	char *prev = *from;

	if (*prev == ':') {
		 
		if (!HTTP_IS_CRLF(*next))
			next++;
		prev++;
		if (prev < next)
			*prev++ = ' ';

		while (HTTP_IS_SPHT(*next))
			next++;
	} else {
		 
		while (HTTP_IS_SPHT(*(prev-1)))
			prev--;
		*from = prev;

		 
		if (!HTTP_IS_CRLF(*next)) {
			*prev++ = *next++;
			if (prev + 1 < next)
				*prev++ = ' ';
			while (HTTP_IS_SPHT(*next))
				next++;
		}
	}
	return buffer_replace2(buf, prev, next, NULL, 0);
}
