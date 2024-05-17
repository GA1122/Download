static int ldb_dn_escape_internal(char *dst, const char *src, int len)
{
	char c;
	char *d;
	int i;
	d = dst;

	for (i = 0; i < len; i++){
		c = src[i];
		switch (c) {
		case ' ':
			if (i == 0 || i == len - 1) {
				 
				*d++ = '\\';
				*d++ = c;
			} else {
				 
				*d++ = c;
			}
			break;

		case '#':
			 
		case ',':
		case '+':
		case '"':
		case '\\':
		case '<':
		case '>':
		case '?':
			 
			*d++ = '\\';
			*d++ = c;
			break;

		case ';':
		case '\r':
		case '\n':
		case '=':
		case '\0': {
			 
			unsigned char v;
			const char *hexbytes = "0123456789ABCDEF";
			v = (const unsigned char)c;
			*d++ = '\\';
			*d++ = hexbytes[v>>4];
			*d++ = hexbytes[v&0xF];
			break;
		}
		default:
			*d++ = c;
		}
	}

	 
	return (d - dst);
}
