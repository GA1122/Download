xsltNumberFormatTokenize(const xmlChar *format,
			 xsltFormatPtr tokens)
{
    int ix = 0;
    int j;
    int val;
    int len;

    default_token.token = DEFAULT_TOKEN;
    default_token.width = 1;
    default_token.separator = BAD_CAST(DEFAULT_SEPARATOR);


    tokens->start = NULL;
    tokens->tokens[0].separator = NULL;
    tokens->end = NULL;

     
    while (! (IS_LETTER(val=xmlStringCurrentChar(NULL, format+ix, &len)) ||
	      IS_DIGIT(val)) ) {
	if (format[ix] == 0)		 
	    break;  
	ix += len;
    }
    if (ix > 0)
	tokens->start = xmlStrndup(format, ix);


    for (tokens->nTokens = 0; tokens->nTokens < MAX_TOKENS;
	 tokens->nTokens++) {
	if (format[ix] == 0)
	    break;  

	 
	if (tokens->nTokens > 0) {
	    tokens->tokens[tokens->nTokens].separator = tokens->end;
	    tokens->end = NULL;
	}

	val = xmlStringCurrentChar(NULL, format+ix, &len);
	if (IS_DIGIT_ONE(val) ||
		 IS_DIGIT_ZERO(val)) {
	    tokens->tokens[tokens->nTokens].width = 1;
	    while (IS_DIGIT_ZERO(val)) {
		tokens->tokens[tokens->nTokens].width++;
		ix += len;
		val = xmlStringCurrentChar(NULL, format+ix, &len);
	    }
	    if (IS_DIGIT_ONE(val)) {
		tokens->tokens[tokens->nTokens].token = val - 1;
		ix += len;
		val = xmlStringCurrentChar(NULL, format+ix, &len);
	    }
	} else if ( (val == (xmlChar)'A') ||
		    (val == (xmlChar)'a') ||
		    (val == (xmlChar)'I') ||
		    (val == (xmlChar)'i') ) {
	    tokens->tokens[tokens->nTokens].token = val;
	    ix += len;
	    val = xmlStringCurrentChar(NULL, format+ix, &len);
	} else {
	     
	    tokens->tokens[tokens->nTokens].token = (xmlChar)'0';
	    tokens->tokens[tokens->nTokens].width = 1;
	}
	 
	while (IS_LETTER(val) || IS_DIGIT(val)) {
	    ix += len;
	    val = xmlStringCurrentChar(NULL, format+ix, &len);
	}

	 
	j = ix;
	while (! (IS_LETTER(val) || IS_DIGIT(val))) {
	    if (val == 0)
		break;  
	    ix += len;
	    val = xmlStringCurrentChar(NULL, format+ix, &len);
	}
	if (ix > j)
	    tokens->end = xmlStrndup(&format[j], ix - j);
    }
}
