print_string(char *s, ssize_t len, int dl, const uint8_t *data)
{
 uint8_t c;
 const uint8_t *e, *p;
 ssize_t bytes = 0;
 ssize_t r;

	e = data + dl;
 while (data < e) {
		c = *data++;
 if (c == '\0') {
  
 for (p = data; p < e; p++)
 if (*p != '\0')
 break;
 if (p == e)
 break;
 }
 if (!isascii(c) || !isprint(c)) {
 if (s) {
 if (len < 5) {
					errno = ENOBUFS;
 return -1;
 }
				r = snprintf(s, len, "\\%03o", c);
				len -= r;
				bytes += r;
				s += r;
 } else
				bytes += 4;
 continue;
 }
 switch (c) {
 case '"':  
 case '\'':  
 case '$':  
 case '`':  
 case '\\':  
 case '|':  
 case '&':
 if (s) {
 if (len < 3) {
					errno = ENOBUFS;
 return -1;
 }
 *s++ = '\\';
				len--;
 }
			bytes++;
 break;
 }
 if (s) {
 *s++ = c;
			len--;
 }
		bytes++;
 }

  
 if (s)
 *s = '\0';
	bytes++;
 return bytes;
}
