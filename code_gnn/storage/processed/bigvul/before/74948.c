next_field(const char **p, const char **start,
    const char **end, char *sep)
{
	 
	while (**p == ' ' || **p == '\t' || **p == '\n') {
		(*p)++;
	}
	*start = *p;

	 
	while (**p != '\0' && **p != ',' && **p != ':' && **p != '\n' &&
	    **p != '#') {
		(*p)++;
	}
	*sep = **p;

	 
	if (*p == *start) {
		*end = *p;
	} else {
		*end = *p - 1;
		while (**end == ' ' || **end == '\t' || **end == '\n') {
			(*end)--;
		}
		(*end)++;
	}

	 
	if (*sep == '#') {
		while (**p != '\0' && **p != ',' && **p != '\n') {
			(*p)++;
		}
		*sep = **p;
	}

	 
	if (**p != '\0')
		(*p)++;
}
