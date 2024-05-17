http_GetHdrField(const struct http *hp, const char *hdr,
    const char *field, char **ptr)
{
	char *h;
	int i;

	if (ptr != NULL)
		*ptr = NULL;

	h = NULL;
	i = http_GetHdrData(hp, hdr, field, &h);
	if (!i)
		return (i);

	if (ptr != NULL && h != NULL) {
		 
		while (*h && vct_issp(*h))
			h++;
		if (*h == '=') {
			h++;
			while (*h && vct_issp(*h))
				h++;
			*ptr = h;
		}
	}
	return (i);
}