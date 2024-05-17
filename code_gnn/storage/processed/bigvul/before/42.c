static PHP_FUNCTION(preg_split)
{
	char				*regex;			 
	char				*subject;		 
	int					 regex_len;
	int					 subject_len;
	long				 limit_val = -1; 
	long				 flags = 0;		 
	pcre_cache_entry	*pce;			 

	 	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|ll", &regex, &regex_len,
							  &subject, &subject_len, &limit_val, &flags) == FAILURE) {
		RETURN_FALSE;
	}
	
	 
	if ((pce = pcre_get_compiled_regex_cache(regex, regex_len TSRMLS_CC)) == NULL) {
		RETURN_FALSE;
	}

	php_pcre_split_impl(pce, subject, subject_len, return_value, limit_val, flags TSRMLS_CC);
}
