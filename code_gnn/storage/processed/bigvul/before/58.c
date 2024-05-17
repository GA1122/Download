PHPAPI void  php_pcre_grep_impl(pcre_cache_entry *pce, zval *input, zval *return_value, long flags TSRMLS_DC)  
{
	zval		   **entry;				 
	pcre_extra		*extra = pce->extra; 
	pcre_extra		 extra_data;		 
	int				*offsets;			 
	int				 size_offsets;		 
	int				 count = 0;			 
	char			*string_key;
	ulong			 num_key;
	zend_bool		 invert;			 
	int				 rc;
	
	invert = flags & PREG_GREP_INVERT ? 1 : 0;
	
	if (extra == NULL) {
		extra_data.flags = PCRE_EXTRA_MATCH_LIMIT | PCRE_EXTRA_MATCH_LIMIT_RECURSION;
		extra = &extra_data;
	}
	extra->match_limit = PCRE_G(backtrack_limit);
	extra->match_limit_recursion = PCRE_G(recursion_limit);

	 
	rc = pcre_fullinfo(pce->re, extra, PCRE_INFO_CAPTURECOUNT, &size_offsets);
	if (rc < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Internal pcre_fullinfo() error %d", rc);
		RETURN_FALSE;
	}
	size_offsets = (size_offsets + 1) * 3;
	offsets = (int *)safe_emalloc(size_offsets, sizeof(int), 0);
	
	 
	array_init(return_value);

	PCRE_G(error_code) = PHP_PCRE_NO_ERROR;

	 
	zend_hash_internal_pointer_reset(Z_ARRVAL_P(input));
	while (zend_hash_get_current_data(Z_ARRVAL_P(input), (void **)&entry) == SUCCESS) {
		zval subject = **entry;

		if (Z_TYPE_PP(entry) != IS_STRING) {
			zval_copy_ctor(&subject);
			convert_to_string(&subject);
		}

		 
		count = pcre_exec(pce->re, extra, Z_STRVAL(subject),
						  Z_STRLEN(subject), 0,
						  0, offsets, size_offsets);

		 
		if (count == 0) {
			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Matched, but too many substrings");
			count = size_offsets/3;
		} else if (count < 0 && count != PCRE_ERROR_NOMATCH) {
			pcre_handle_exec_error(count TSRMLS_CC);
			break;
		}

		 
		if ((count > 0 && !invert) || (count == PCRE_ERROR_NOMATCH && invert)) {

			Z_ADDREF_PP(entry);

			 
			switch (zend_hash_get_current_key(Z_ARRVAL_P(input), &string_key, &num_key, 0))
			{
				case HASH_KEY_IS_STRING:
					zend_hash_update(Z_ARRVAL_P(return_value), string_key,
									 strlen(string_key)+1, entry, sizeof(zval *), NULL);
					break;

				case HASH_KEY_IS_LONG:
					zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, entry,
										   sizeof(zval *), NULL);
					break;
			}
		}

		if (Z_TYPE_PP(entry) != IS_STRING) {
			zval_dtor(&subject);
		}

		zend_hash_move_forward(Z_ARRVAL_P(input));
	}
	zend_hash_internal_pointer_reset(Z_ARRVAL_P(input));
	 
	efree(offsets);
}
 
