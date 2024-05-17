int not_unsigned(const char *str) {
	EUID_ASSERT();

	int rv = 0;
	const char *ptr = str;
	while (*ptr != ' ' && *ptr != '\t' && *ptr != '\0') {
		if (!isdigit(*ptr)) {
			rv = 1;
			break;
		}
		ptr++;
	}

	return rv;
}
