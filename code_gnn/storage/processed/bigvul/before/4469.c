int phar_open_from_filename(char *fname, int fname_len, char *alias, int alias_len, int options, phar_archive_data** pphar, char **error TSRMLS_DC)  
{
	php_stream *fp;
	char *actual;
	int ret, is_data = 0;

	if (error) {
		*error = NULL;
	}

	if (!strstr(fname, ".phar")) {
		is_data = 1;
	}

	if (phar_open_parsed_phar(fname, fname_len, alias, alias_len, is_data, options, pphar, error TSRMLS_CC) == SUCCESS) {
		return SUCCESS;
	} else if (error && *error) {
		return FAILURE;
	}
#if PHP_API_VERSION < 20100412
	if (PG(safe_mode) && (!php_checkuid(fname, NULL, CHECKUID_ALLOW_ONLY_FILE))) {
		return FAILURE;
	}
#endif
	if (php_check_open_basedir(fname TSRMLS_CC)) {
		return FAILURE;
	}

	fp = php_stream_open_wrapper(fname, "rb", IGNORE_URL|STREAM_MUST_SEEK, &actual);

	if (!fp) {
		if (options & REPORT_ERRORS) {
			if (error) {
				spprintf(error, 0, "unable to open phar for reading \"%s\"", fname);
			}
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

	ret =  phar_open_from_fp(fp, fname, fname_len, alias, alias_len, options, pphar, is_data, error TSRMLS_CC);

	if (actual) {
		efree(actual);
	}

	return ret;
}
 