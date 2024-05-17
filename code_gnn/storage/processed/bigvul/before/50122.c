PHP_FUNCTION(curl_setopt_array)
{
	zval		*zid, *arr, *entry;
	php_curl	*ch;
	zend_ulong	option;
	zend_string	*string_key;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ra", &zid, &arr) == FAILURE) {
		return;
	}

	if ((ch = (php_curl*)zend_fetch_resource(Z_RES_P(zid), le_curl_name, le_curl)) == NULL) {
		RETURN_FALSE;
	}

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(arr), option, string_key, entry) {
		if (string_key) {
			php_error_docref(NULL, E_WARNING,
					"Array keys must be CURLOPT constants or equivalent integer values");
			RETURN_FALSE;
		}
		if (_php_curl_setopt(ch, (zend_long) option, entry) == FAILURE) {
			RETURN_FALSE;
		}
	} ZEND_HASH_FOREACH_END();

	RETURN_TRUE;
}