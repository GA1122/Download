static PHP_FUNCTION(preg_quote)
{
	int		 in_str_len;
	char	*in_str;		 
	char	*in_str_end;     
	int		 delim_len = 0;
	char	*delim = NULL;	 
	char	*out_str,		 
		 	*p,				 
			*q,				 
			 delim_char=0,	 
			 c;				 
	zend_bool quote_delim = 0;  
	
	 
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &in_str, &in_str_len,
							  &delim, &delim_len) == FAILURE) {
		return;
	}
	
	in_str_end = in_str + in_str_len;

	 
	if (in_str == in_str_end) {
		RETURN_EMPTY_STRING();
	}

	if (delim && *delim) {
		delim_char = delim[0];
		quote_delim = 1;
	}
	
	 
	out_str = safe_emalloc(4, in_str_len, 1);
	
	 
	for(p = in_str, q = out_str; p != in_str_end; p++) {
		c = *p;
		switch(c) {
			case '.':
			case '\\':
			case '+':
			case '*':
			case '?':
			case '[':
			case '^':
			case ']':
			case '$':
			case '(':
			case ')':
			case '{':
			case '}':
			case '=':
			case '!':
			case '>':
			case '<':
			case '|':
			case ':':
			case '-':
				*q++ = '\\';
				*q++ = c;
				break;

			case '\0':
				*q++ = '\\';
				*q++ = '0';
				*q++ = '0';
				*q++ = '0';
				break;

			default:
				if (quote_delim && c == delim_char)
					*q++ = '\\';
				*q++ = c;
				break;
		}
	}
	*q = '\0';
	
	 
	RETVAL_STRINGL(erealloc(out_str, q - out_str + 1), q - out_str, 0);
}
