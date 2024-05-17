PHP_FUNCTION(fgetcsv)
{
	char delimiter = ',';	 
	char enclosure = '"';	 
	char escape = '\\';

	 

	long len = 0;
	size_t buf_len;
	char *buf;
	php_stream *stream;

	{
		zval *fd, **len_zv = NULL;
		char *delimiter_str = NULL;
		int delimiter_str_len = 0;
		char *enclosure_str = NULL;
		int enclosure_str_len = 0;
		char *escape_str = NULL;
		int escape_str_len = 0;

		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|Zsss",
			&fd, &len_zv, &delimiter_str, &delimiter_str_len,
			&enclosure_str, &enclosure_str_len,
			&escape_str, &escape_str_len) == FAILURE
		) {
			return;
		}

		if (delimiter_str != NULL) {
			 
			if (delimiter_str_len < 1) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "delimiter must be a character");
				RETURN_FALSE;
			} else if (delimiter_str_len > 1) {
				php_error_docref(NULL TSRMLS_CC, E_NOTICE, "delimiter must be a single character");
			}

			 
			delimiter = delimiter_str[0];
		}

		if (enclosure_str != NULL) {
			if (enclosure_str_len < 1) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "enclosure must be a character");
				RETURN_FALSE;
			} else if (enclosure_str_len > 1) {
				php_error_docref(NULL TSRMLS_CC, E_NOTICE, "enclosure must be a single character");
			}

			 
			enclosure = enclosure_str[0];
		}

		if (escape_str != NULL) {
			if (escape_str_len < 1) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "escape must be character");
				RETURN_FALSE;
			} else if (escape_str_len > 1) {
				php_error_docref(NULL TSRMLS_CC, E_NOTICE, "escape must be a single character");
			}

			escape = escape_str[0];
		}

		if (len_zv != NULL && Z_TYPE_PP(len_zv) != IS_NULL) {
			convert_to_long_ex(len_zv);
			len = Z_LVAL_PP(len_zv);
			if (len < 0) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Length parameter may not be negative");
				RETURN_FALSE;
			} else if (len == 0) {
				len = -1;
			}
		} else {
			len = -1;
		}

		PHP_STREAM_TO_ZVAL(stream, &fd);
	}

	if (len < 0) {
		if ((buf = php_stream_get_line(stream, NULL, 0, &buf_len)) == NULL) {
			RETURN_FALSE;
		}
	} else {
		buf = emalloc(len + 1);
		if (php_stream_get_line(stream, buf, len + 1, &buf_len) == NULL) {
			efree(buf);
			RETURN_FALSE;
		}
	}

	php_fgetcsv(stream, delimiter, enclosure, escape, buf_len, buf, return_value TSRMLS_CC);
}
