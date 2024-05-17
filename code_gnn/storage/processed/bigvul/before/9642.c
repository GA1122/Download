PHPAPI int php_session_valid_key(const char *key)  
{
	size_t len;
	const char *p;
	char c;
	int ret = SUCCESS;

	for (p = key; (c = *p); p++) {
		 
		if (!((c >= 'a' && c <= 'z')
				|| (c >= 'A' && c <= 'Z')
				|| (c >= '0' && c <= '9')
				|| c == ','
				|| c == '-')) {
			ret = FAILURE;
			break;
		}
	}

	len = p - key;

	 
	if (len == 0 || len > 128) {
		ret = FAILURE;
	}

	return ret;
}
 
