PHP_FUNCTION(getenv)
{
	char *ptr, *str;
	int str_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len) == FAILURE) {
		RETURN_FALSE;
	}

	 
	ptr = sapi_getenv(str, str_len TSRMLS_CC);
	if (ptr) {
		RETURN_STRING(ptr, 0);
	}
#ifdef PHP_WIN32
	{
		char dummybuf;
		int size;

		SetLastError(0);
		 
		size = GetEnvironmentVariableA(str, &dummybuf, 0);
		if (GetLastError() == ERROR_ENVVAR_NOT_FOUND) {
				 
				RETURN_FALSE;
		}

		if (size == 0) {
				 
				RETURN_EMPTY_STRING();
		}

		ptr = emalloc(size);
		size = GetEnvironmentVariableA(str, ptr, size);
		if (size == 0) {
				 
				efree(ptr);
				RETURN_EMPTY_STRING();
		} else {
			RETURN_STRING(ptr, 0);
		}
	}
#else
	 
	ptr = getenv(str);
	if (ptr) {
		RETURN_STRING(ptr, 1);
	}
#endif
	RETURN_FALSE;
}
