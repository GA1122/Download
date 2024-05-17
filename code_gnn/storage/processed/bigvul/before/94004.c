static inline void sanitize_rfc5987(zval *zv, char **language, zend_bool *latin1 TSRMLS_DC)
{
	char *ptr;

	 

	switch (Z_STRVAL_P(zv)[0]) {
	case 'I':
	case 'i':
		if (!strncasecmp(Z_STRVAL_P(zv), "iso-8859-1", lenof("iso-8859-1"))) {
			*latin1 = 1;
			ptr = Z_STRVAL_P(zv) + lenof("iso-8859-1");
			break;
		}
		 
	case 'U':
	case 'u':
		if (!strncasecmp(Z_STRVAL_P(zv), "utf-8", lenof("utf-8"))) {
			*latin1 = 0;
			ptr = Z_STRVAL_P(zv) + lenof("utf-8");
			break;
		}
		 
	default:
		return;
	}

	 
	if (*ptr == '\'') {
		for (*language = ++ptr; *ptr && *ptr != '\''; ++ptr);
		if (!*ptr) {
			*language = NULL;
			return;
		}
		*language = estrndup(*language, ptr - *language);

		 
		ptr = estrdup(++ptr);
		zval_dtor(zv);
		ZVAL_STRING(zv, ptr, 0);
	}
}
