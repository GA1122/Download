http_get_path_from_string(char *str)
{
	char *ptr = str;

	 

	if (*ptr == '*')
		return NULL;

	if (isalpha((unsigned char)*ptr)) {
		 
		ptr++;
		while (isalnum((unsigned char)*ptr) || *ptr == '+' || *ptr == '-' || *ptr == '.')
			ptr++;
		 
		if (*ptr == '\0' || *ptr++ != ':')
			return NULL;
		if (*ptr == '\0' || *ptr++ != '/')
			return NULL;
		if (*ptr == '\0' || *ptr++ != '/')
			return NULL;
	}
	 

	while (*ptr != '\0' && *ptr != ' ' && *ptr != '/')
		ptr++;

	if (*ptr == '\0' || *ptr == ' ')
		return NULL;

	 
	return ptr;
}
