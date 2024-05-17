int phar_open_executed_filename(char *alias, int alias_len, char **error TSRMLS_DC)  
{
	char *fname;
	zval *halt_constant;
	php_stream *fp;
	int fname_len;
	char *actual = NULL;
	int ret;

	if (error) {
		*error = NULL;
	}

	fname = (char*)zend_get_executed_filename(TSRMLS_C);
	fname_len = strlen(fname);

	if (phar_open_parsed_phar(fname, fname_len, alias, alias_len, 0, REPORT_ERRORS, NULL, 0 TSRMLS_CC) == SUCCESS) {
		return SUCCESS;
	}

	if (!strcmp(fname, "[no active file]")) {
		if (error) {
			spprintf(error, 0, "cannot initialize a phar outside of PHP execution");
		}
		return FAILURE;
	}

	MAKE_STD_ZVAL(halt_constant);

	if (0 == zend_get_constant("__COMPILER_HALT_OFFSET__", 24, halt_constant TSRMLS_CC)) {
		FREE_ZVAL(halt_constant);
		if (error) {
			spprintf(error, 0, "__HALT_COMPILER(); must be declared in a phar");
		}
		return FAILURE;
	}

	FREE_ZVAL(halt_constant);

#if PHP_API_VERSION < 20100412
	if (PG(safe_mode) && (!php_checkuid(fname, NULL, CHECKUID_ALLOW_ONLY_FILE))) {
		return FAILURE;
	}
#endif

	if (php_check_open_basedir(fname TSRMLS_CC)) {
		return FAILURE;
	}

	fp = php_stream_open_wrapper(fname, "rb", IGNORE_URL|STREAM_MUST_SEEK|REPORT_ERRORS, &actual);

	if (!fp) {
		if (error) {
			spprintf(error, 0, "unable to open phar for reading \"%s\"", fname);
		}
		if (actual) {
			efree(actual);
		}
		return FAILURE;
	}

	if (actual) {
		fname = actual;
		fname_len = strlen(actual);
	}

	ret = phar_open_from_fp(fp, fname, fname_len, alias, alias_len, REPORT_ERRORS, NULL, 0, error TSRMLS_CC);

	if (actual) {
		efree(actual);
	}

	return ret;
}
 