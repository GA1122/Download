http_GetHdrData(const struct http *hp, const char *hdr,
    const char *field, char **ptr)
{
	char *h, *e;
	unsigned fl;

	if (ptr != NULL)
		*ptr = NULL;
	if (!http_GetHdr(hp, hdr, &h))
		return (0);
	AN(h);
	e = strchr(h, '\0');
	fl = strlen(field);
	while (h + fl <= e) {
		 
		if (vct_islws(*h) || *h == ',') {
			h++;
			continue;
		}
		 
		if ((h + fl == e || vct_issepctl(h[fl])) &&
		    !memcmp(h, field, fl)) {
			if (ptr != NULL) {
				h += fl;
				while (vct_islws(*h))
					h++;
				*ptr = h;
			}
			return (1);
		}
		 
		while (*h && *h != ',')
			h++;
	}
	return (0);
}
