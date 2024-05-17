mkdbname(struct magic_set *ms, const char *fn, int strip)
{
	const char *p, *q;
	char *buf;

	if (strip) {
		if ((p = strrchr(fn, '/')) != NULL)
			fn = ++p;
	}

	for (q = fn; *q; q++)
		continue;
	 
	for (p = ext + sizeof(ext) - 1; p >= ext && q >= fn; p--, q--)
		if (*p != *q)
			break;

	 
	if (p >= ext)
		while (*q)
			q++;

	q++;
	 
	if (ms->flags & MAGIC_MIME) {
		if (asprintf(&buf, "%.*s.mime%s", (int)(q - fn), fn, ext) < 0)
			return NULL;
		if (access(buf, R_OK) != -1) {
			ms->flags &= MAGIC_MIME_TYPE;
			return buf;
		}
		free(buf);
	}
	if (asprintf(&buf, "%.*s%s", (int)(q - fn), fn, ext) < 0)
		return NULL;

	 
	if (strstr(p, ".mime") != NULL)
		ms->flags &= MAGIC_MIME_TYPE;
	return buf;
}
