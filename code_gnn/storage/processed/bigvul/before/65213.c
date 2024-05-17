static const char *nlmdbg_cookie2a(const struct nlm_cookie *cookie)
{
	 
	static char buf[2*NLM_MAXCOOKIELEN+1];
	unsigned int i, len = sizeof(buf);
	char *p = buf;

	len--;	 
	if (len < 3)
		return "???";
	for (i = 0 ; i < cookie->len ; i++) {
		if (len < 2) {
			strcpy(p-3, "...");
			break;
		}
		sprintf(p, "%02x", cookie->data[i]);
		p += 2;
		len -= 2;
	}
	*p = '\0';

	return buf;
}
