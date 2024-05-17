int parse_uint(const char *nptr, char **endptr, int base, guint *number)
{
	char *endptr_;
	gulong parsed;

	 
	if (!i_isdigit(*nptr)) {
		return FALSE;
	}

	errno = 0;
	parsed = strtoul(nptr, &endptr_, base);

	if (errno || endptr_ == nptr || parsed >= (1U << 31)) {
		return FALSE;
	}

	if (endptr) {
		*endptr = endptr_;
	}

	if (number) {
		*number = (guint) parsed;
	}

	return TRUE;
}