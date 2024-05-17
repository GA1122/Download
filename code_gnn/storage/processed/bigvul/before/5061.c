FindFirst(char *string, char dest, int *lines)
{
    if (lines)
	*lines = 0;
    for (;;) {
	if (*string == '\0')
	    return NULL;
	if (*string == '\\') {
	    if (*++string == '\0')
		return NULL;
	} else if (*string == dest)
	    return string;
	if (*string == '\n'  &&  lines)
	    (*lines)++;
	string++;
    }
}
