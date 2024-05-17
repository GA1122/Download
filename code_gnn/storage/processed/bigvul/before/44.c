static PHP_FUNCTION(preg_grep)
{
	char				*regex;			 
	int				 	 regex_len;
	zval				*input;			 
	long				 flags = 0;		 
	pcre_cache_entry	*pce;			 

	 
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa|l", &regex, &regex_len,
							  &input, &flags) == FAILURE) {
		return;
	}
	
	 
	if ((pce = pcre_get_compiled_regex_cache(regex, regex_len TSRMLS_CC)) == NULL) {
		RETURN_FALSE;
	}
	
	php_pcre_grep_impl(pce, input, return_value, flags TSRMLS_CC);
}
