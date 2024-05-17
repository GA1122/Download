static int preg_do_eval(char *eval_str, int eval_str_len, char *subject,
						int *offsets, int count, char **result TSRMLS_DC)
{
	zval		 retval;			 
	char		*eval_str_end,		 
				*match,				 
				*esc_match,			 
				*walk,				 
				*segment,			 
				 walk_last;			 
	int			 match_len;			 
	int			 esc_match_len;		 
	int			 result_len;		 
	int			 backref;			 
	char        *compiled_string_description;
	smart_str    code = {0};
	
	eval_str_end = eval_str + eval_str_len;
	walk = segment = eval_str;
	walk_last = 0;
	
	while (walk < eval_str_end) {
		 
		if ('\\' == *walk || '$' == *walk) {
			smart_str_appendl(&code, segment, walk - segment);
			if (walk_last == '\\') {
				code.c[code.len-1] = *walk++;
				segment = walk;
				walk_last = 0;
				continue;
			}
			segment = walk;
			if (preg_get_backref(&walk, &backref)) {
				if (backref < count) {
					 
					match = subject + offsets[backref<<1];
					match_len = offsets[(backref<<1)+1] - offsets[backref<<1];
					if (match_len) {
						esc_match = php_addslashes(match, match_len, &esc_match_len, 0 TSRMLS_CC);
					} else {
						esc_match = match;
						esc_match_len = 0;
					}
				} else {
					esc_match = "";
					esc_match_len = 0;
				}
				smart_str_appendl(&code, esc_match, esc_match_len);

				segment = walk;

				 
				if (esc_match_len)
					efree(esc_match);
				continue;
			}
		}
		walk++;
		walk_last = walk[-1];
	}
	smart_str_appendl(&code, segment, walk - segment);
	smart_str_0(&code);

	compiled_string_description = zend_make_compiled_string_description("regexp code" TSRMLS_CC);
	 
	if (zend_eval_stringl(code.c, code.len, &retval, compiled_string_description TSRMLS_CC) == FAILURE) {
		efree(compiled_string_description);
		php_error_docref(NULL TSRMLS_CC,E_ERROR, "Failed evaluating code: %s%s", PHP_EOL, code.c);
		 
	}
	efree(compiled_string_description);
	convert_to_string(&retval);
	
	 
	*result = estrndup(Z_STRVAL(retval), Z_STRLEN(retval));
	result_len = Z_STRLEN(retval);
	
	 
	zval_dtor(&retval);
	smart_str_free(&code);
	
	return result_len;
}
