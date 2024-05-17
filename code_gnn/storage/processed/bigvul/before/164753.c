htmlParseNameComplex(xmlParserCtxtPtr ctxt) {
    int len = 0, l;
    int c;
    int count = 0;
    const xmlChar *base = ctxt->input->base;

     
    GROW;
    c = CUR_CHAR(l);
    if ((c == ' ') || (c == '>') || (c == '/') ||  
	(!IS_LETTER(c) && (c != '_') &&
         (c != ':'))) {
	return(NULL);
    }

    while ((c != ' ') && (c != '>') && (c != '/') &&  
	   ((IS_LETTER(c)) || (IS_DIGIT(c)) ||
            (c == '.') || (c == '-') ||
	    (c == '_') || (c == ':') ||
	    (IS_COMBINING(c)) ||
	    (IS_EXTENDER(c)))) {
	if (count++ > 100) {
	    count = 0;
	    GROW;
	}
	len += l;
	NEXTL(l);
	c = CUR_CHAR(l);
	if (ctxt->input->base != base) {
	     
	    return(htmlParseNameComplex(ctxt));
	}
    }

    if (ctxt->input->cur - ctxt->input->base < len) {
         
	htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
                     "unexpected change of input buffer", NULL, NULL);
        return (NULL);
    }

    return(xmlDictLookup(ctxt->dict, ctxt->input->cur - len, len));
}
