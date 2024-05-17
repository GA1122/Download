alloc_strvec_r(char *string)
{
	char *cp, *start, *token;
	size_t str_len;
	vector_t *strvec;

	if (!string)
		return NULL;

	 
	strvec = vector_alloc();

	cp = string;
	while (true) {
		cp += strspn(cp, WHITE_SPACE);
		if (!*cp)
			break;

		start = cp;

		 
		if (*start == '"') {
			start++;
			if (!(cp = strchr(start, '"'))) {
				report_config_error(CONFIG_UNMATCHED_QUOTE, "Unmatched quote: '%s'", string);
				break;
			}
			str_len = (size_t)(cp - start);
			cp++;
		} else {
			cp += strcspn(start, WHITE_SPACE_STR "\"");
			str_len = (size_t)(cp - start);
		}
		token = MALLOC(str_len + 1);
		memcpy(token, start, str_len);
		token[str_len] = '\0';

		 
		vector_alloc_slot(strvec);
		vector_set_slot(strvec, token);
	}

	if (!vector_size(strvec)) {
		free_strvec(strvec);
		return NULL;
	}

	return strvec;
}