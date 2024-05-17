static int phar_check_str(const char *fname, const char *ext_str, int ext_len, int executable, int for_create)  
{
	char test[51];
	const char *pos;

	if (ext_len >= 50) {
		return FAILURE;
	}

	if (executable == 1) {
		 
		memcpy(test, ext_str - 1, ext_len + 1);
		test[ext_len + 1] = '\0';
		 
		 
		pos = strstr(test, ".phar");

		if (pos && (*(pos - 1) != '/')
				&& (pos += 5) && (*pos == '\0' || *pos == '/' || *pos == '.')) {
			return phar_analyze_path(fname, ext_str, ext_len, for_create);
		} else {
			return FAILURE;
		}
	}

	 
	if (!executable) {
		pos = strstr(ext_str, ".phar");
		if (!(pos && (*(pos - 1) != '/')
					&& (pos += 5) && (*pos == '\0' || *pos == '/' || *pos == '.')) && *(ext_str + 1) != '.' && *(ext_str + 1) != '/' && *(ext_str + 1) != '\0') {
			return phar_analyze_path(fname, ext_str, ext_len, for_create);
		}
	} else {
		if (*(ext_str + 1) != '.' && *(ext_str + 1) != '/' && *(ext_str + 1) != '\0') {
			return phar_analyze_path(fname, ext_str, ext_len, for_create);
		}
	}

	return FAILURE;
}
 
