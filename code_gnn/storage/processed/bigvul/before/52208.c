static inline int process_named_entity_html(const char **buf, const char **start, size_t *length)
{
	*start = *buf;

	 
	while ((**buf >= 'a' && **buf <= 'z') ||
			(**buf >= 'A' && **buf <= 'Z') ||
			(**buf >= '0' && **buf <= '9')) {
		(*buf)++;
	}

	if (**buf != ';')
		return FAILURE;

	 
	*length = *buf - *start;

	if (*length == 0)
		return FAILURE;

	return SUCCESS;
}
