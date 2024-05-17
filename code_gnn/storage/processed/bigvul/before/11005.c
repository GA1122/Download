PHPAPI char *xml_utf8_decode(const XML_Char *s, int len, int *newlen, const XML_Char *encoding)
{
	size_t pos = 0;
	char *newbuf = emalloc(len + 1);
	unsigned int c;
	char (*decoder)(unsigned short) = NULL;
	xml_encoding *enc = xml_get_encoding(encoding);

	*newlen = 0;
	if (enc) {
		decoder = enc->decoding_function;
	}
	if (decoder == NULL) {
		 
		memcpy(newbuf, s, len);
		*newlen = len;
		newbuf[*newlen] = '\0';
		return newbuf;
	}

	while (pos < (size_t)len) {
		int status = FAILURE;
		c = php_next_utf8_char((const unsigned char*)s, (size_t) len, &pos, &status);

		if (status == FAILURE || c > 0xFFU) {
			c = '?';
		}

		newbuf[*newlen] = decoder ? decoder(c) : c;
		++*newlen;
	}
	if (*newlen < len) {
		newbuf = erealloc(newbuf, *newlen + 1);
	}
	newbuf[*newlen] = '\0';
	return newbuf;
}
