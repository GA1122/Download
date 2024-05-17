get_option_string(const struct dhcp_message *dhcp, uint8_t option)
{
 int type = 0;
 int len;
 const uint8_t *p;
 char *s;

	p = get_option(dhcp, option, &len, &type);
 if (!p || *p == '\0')
 return NULL;

 if (type & RFC3397) {
		type = decode_rfc3397(NULL, 0, len, p);
 if (!type) {
			errno = EINVAL;
 return NULL;
 }
		s = xmalloc(sizeof(char) * type);
		decode_rfc3397(s, type, len, p);
 return s;
 }

 if (type & RFC3361)
 return decode_rfc3361(len, p);

	s = xmalloc(sizeof(char) * (len + 1));
	memcpy(s, p, len);
	s[len] = '\0';
 return s;
}
