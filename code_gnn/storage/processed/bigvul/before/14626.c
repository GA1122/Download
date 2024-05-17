PHP_FUNCTION(chunk_split)
{
	zend_string *str;
	char *end    = "\r\n";
	size_t endlen   = 2;
	zend_long chunklen = 76;
	zend_string *result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|ls", &str, &chunklen, &end, &endlen) == FAILURE) {
		return;
	}

	if (chunklen <= 0) {
		php_error_docref(NULL, E_WARNING, "Chunk length should be greater than zero");
		RETURN_FALSE;
	}

	if ((size_t)chunklen > ZSTR_LEN(str)) {
		 
		result = zend_string_alloc(endlen + ZSTR_LEN(str), 0);
		memcpy(ZSTR_VAL(result), ZSTR_VAL(str), ZSTR_LEN(str));
		memcpy(ZSTR_VAL(result) + ZSTR_LEN(str), end, endlen);
		ZSTR_VAL(result)[ZSTR_LEN(result)] = '\0';
		RETURN_NEW_STR(result);
	}

	if (!ZSTR_LEN(str)) {
		RETURN_EMPTY_STRING();
	}

	result = php_chunk_split(ZSTR_VAL(str), ZSTR_LEN(str), end, endlen, (size_t)chunklen);

	if (result) {
		RETURN_STR(result);
	} else {
		RETURN_FALSE;
	}
}
