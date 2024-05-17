int sc_asn1_put_tag(unsigned int tag, const u8 * data, size_t datalen, u8 * out, size_t outlen, u8 **ptr)
{
	size_t c = 0;
	size_t tag_len;
	size_t ii;
	u8 *p = out;
	u8 tag_char[4] = {0, 0, 0, 0};

	 
	if (tag == 0 || tag > 0xFFFFFFFF) {
		 
		return SC_ERROR_INVALID_DATA;
	}
	for (tag_len = 0; tag; tag >>= 8) {
		 
		tag_char[tag_len++] = tag & 0xFF;
	}

	if (tag_len > 1)   {
		if ((tag_char[tag_len - 1] & SC_ASN1_TAG_PRIMITIVE) != SC_ASN1_TAG_ESCAPE_MARKER) {
			 
			return SC_ERROR_INVALID_DATA;
		}
		for (ii = 1; ii < tag_len - 1; ii++) {
			if ((tag_char[ii] & 0x80) != 0x80) {
				 
				return SC_ERROR_INVALID_DATA;
			}
		}
		if ((tag_char[0] & 0x80) != 0x00) {
			 
			return SC_ERROR_INVALID_DATA;
		}
	}

	 
	 
	if (datalen > 127) {
		c = 1;
		while (datalen >> (c << 3))
			c++;
	}
	if (outlen == 0 || out == NULL) {
		 
		return tag_len + (c+1) + datalen;
	}
	 
	if (outlen < tag_len + (c+1) + datalen)
		return SC_ERROR_BUFFER_TOO_SMALL;
	for (ii=0;ii<tag_len;ii++)
		*p++ = tag_char[tag_len - ii - 1];

	if (c > 0) {
		*p++ = 0x80 | c;
		while (c--)
			*p++ = (datalen >> (c << 3)) & 0xFF;
	}
	else {
		*p++ = datalen & 0x7F;
	}
	if(data && datalen > 0) {
		memcpy(p, data, datalen);
		p += datalen;
	}
	if (ptr != NULL)
		*ptr = p;
	return 0;
}
