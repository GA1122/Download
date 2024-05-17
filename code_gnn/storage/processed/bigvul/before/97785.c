xmlXPathScanName(xmlXPathParserContextPtr ctxt) {
    int len = 0, l;
    int c;
    const xmlChar *cur;
    xmlChar *ret;

    cur = ctxt->cur;

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
	len += l;
	NEXTL(l);
	c = CUR_CHAR(l);
    }
    ret = xmlStrndup(cur, ctxt->cur - cur);
    ctxt->cur = cur;
    return(ret);
}
