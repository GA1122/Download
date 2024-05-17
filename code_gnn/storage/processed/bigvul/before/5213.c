static int _php_pgsql_detect_identifier_escape(const char *identifier, size_t len)  
{
	size_t i;

	 
	if (len <= 2) {
		return FAILURE;
	}
	 
	if (identifier[0] == '"' && identifier[len-1] == '"') {
		 
		for (i = 1; i < len-1; i++) {
			if (identifier[i] == '"' && (identifier[++i] != '"' || i == len-1)) {
				return FAILURE;
			}
		}
	} else {
		return FAILURE;
	}
	 
	return SUCCESS;
}
 
