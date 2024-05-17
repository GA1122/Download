PHP_FUNCTION(snmp_read_mib)
{
	char *filename;
	int filename_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "p", &filename, &filename_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (!read_mib(filename)) {
		char *error = strerror(errno);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Error while reading MIB file '%s': %s", filename, error);
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
