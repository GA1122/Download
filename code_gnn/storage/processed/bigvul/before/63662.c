static int parse_size_uint(const char *size, guint *bytes)
{
	const char *desc;
	guint number, multiplier, limit;
	int len;

	*bytes = 0;

	 
	number = 0;
	while (*size != '\0') {
		if (i_isdigit(*size)) {
			char *endptr;
			if (!parse_uint(size, &endptr, 10, &number)) {
				return FALSE;
			}
			size = endptr;
			continue;
		}

		 
		while (*size != '\0' && i_ispunct(*size))
			size++;

		 
		for (len = 0, desc = size; i_isalpha(*size); size++)
			len++;

		if (len == 0) {
			if (number == 0) {
				 
				return TRUE;
			}

			*bytes += number*1024;  
			return FALSE;
		}

		multiplier = 0;
		limit = 0;

		if (g_ascii_strncasecmp(desc, "gbytes", len) == 0) {
			multiplier = 1U << 30;
			limit = 2U << 0;
		}
		if (g_ascii_strncasecmp(desc, "mbytes", len) == 0) {
			multiplier = 1U << 20;
			limit = 2U << 10;
		}
		if (g_ascii_strncasecmp(desc, "kbytes", len) == 0) {
			multiplier = 1U << 10;
			limit = 2U << 20;
		}
		if (g_ascii_strncasecmp(desc, "bytes", len) == 0) {
			multiplier = 1;
			limit = 2U << 30;
		}

		if (limit && number > limit) {
			return FALSE;
		}

		*bytes += number * multiplier;

		 
		while (*size != '\0' && i_ispunct(*size))
			size++;
	}

	return TRUE;
}
