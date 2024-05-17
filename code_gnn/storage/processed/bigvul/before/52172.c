PHP_FUNCTION(fputcsv)
{
	char delimiter = ',';	  
	char enclosure = '"';	  
	char escape_char = '\\';  
	php_stream *stream;
	zval *fp = NULL, *fields = NULL;
	int ret;
	char *delimiter_str = NULL, *enclosure_str = NULL, *escape_str = NULL;
	int delimiter_str_len = 0, enclosure_str_len = 0, escape_str_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ra|sss",
			&fp, &fields, &delimiter_str, &delimiter_str_len,
			&enclosure_str, &enclosure_str_len,
			&escape_str, &escape_str_len) == FAILURE) {
		return;
	}

	if (delimiter_str != NULL) {
		 
		if (delimiter_str_len < 1) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "delimiter must be a character");
			RETURN_FALSE;
		} else if (delimiter_str_len > 1) {
			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "delimiter must be a single character");
		}

		 
		delimiter = *delimiter_str;
	}

	if (enclosure_str != NULL) {
		if (enclosure_str_len < 1) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "enclosure must be a character");
			RETURN_FALSE;
		} else if (enclosure_str_len > 1) {
			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "enclosure must be a single character");
		}
		 
		enclosure = *enclosure_str;
	}

	if (escape_str != NULL) {
		if (escape_str_len < 1) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "escape must be a character");
			RETURN_FALSE;
		} else if (escape_str_len > 1) {
			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "escape must be a single character");
		}
		 
		escape_char = *escape_str;
	}

	PHP_STREAM_TO_ZVAL(stream, &fp);

	ret = php_fputcsv(stream, fields, delimiter, enclosure, escape_char TSRMLS_CC);
	RETURN_LONG(ret);
}
