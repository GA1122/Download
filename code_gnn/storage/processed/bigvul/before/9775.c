int del_hdr_value(struct buffer *buf, char **from, char *next)
{
	char *prev = *from;

	if (*prev == ':') {
		 
		if (!http_is_crlf[(unsigned char)*next])
			next++;
		prev++;
		if (prev < next)
			*prev++ = ' ';

		while (http_is_spht[(unsigned char)*next])
			next++;
	} else {
		 
		while (http_is_spht[(unsigned char)*(prev-1)])
			prev--;
		*from = prev;

		 
		if (!http_is_crlf[(unsigned char)*next]) {
			*prev++ = *next++;
			if (prev + 1 < next)
				*prev++ = ' ';
			while (http_is_spht[(unsigned char)*next])
				next++;
		}
	}
	return buffer_replace2(buf, prev, next, NULL, 0);
}
