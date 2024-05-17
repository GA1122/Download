kex_names_cat(const char *a, const char *b)
{
	char *ret = NULL, *tmp = NULL, *cp, *p;
	size_t len;

	if (a == NULL || *a == '\0')
		return NULL;
	if (b == NULL || *b == '\0')
		return strdup(a);
	if (strlen(b) > 1024*1024)
		return NULL;
	len = strlen(a) + strlen(b) + 2;
	if ((tmp = cp = strdup(b)) == NULL ||
	    (ret = calloc(1, len)) == NULL) {
		free(tmp);
		return NULL;
	}
	strlcpy(ret, a, len);
	for ((p = strsep(&cp, ",")); p && *p != '\0'; (p = strsep(&cp, ","))) {
		if (match_list(ret, p, NULL) != NULL)
			continue;  
		if (strlcat(ret, ",", len) >= len ||
		    strlcat(ret, p, len) >= len) {
			free(tmp);
			free(ret);
			return NULL;  
		}
	}
	free(tmp);
	return ret;
}