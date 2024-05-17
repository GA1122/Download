char *lxc_string_replace(const char *needle, const char *replacement, const char *haystack)
{
	ssize_t len = -1, saved_len = -1;
	char *result = NULL;
	size_t replacement_len = strlen(replacement);
	size_t needle_len = strlen(needle);

	 
	while (len == -1 || result == NULL) {
		char *p;
		char *last_p;
		ssize_t part_len;

		if (len != -1) {
			result = calloc(1, len + 1);
			if (!result)
				return NULL;
			saved_len = len;
		}

		len = 0;

		for (last_p = (char *)haystack, p = strstr(last_p, needle); p; last_p = p, p = strstr(last_p, needle)) {
			part_len = (ssize_t)(p - last_p);
			if (result && part_len > 0)
				memcpy(&result[len], last_p, part_len);
			len += part_len;
			if (result && replacement_len > 0)
				memcpy(&result[len], replacement, replacement_len);
			len += replacement_len;
			p += needle_len;
		}
		part_len = strlen(last_p);
		if (result && part_len > 0)
			memcpy(&result[len], last_p, part_len);
		len += part_len;
	}

	 
	assert(saved_len == len);
	 
	assert(result[len] == '\0');

	return result;
}