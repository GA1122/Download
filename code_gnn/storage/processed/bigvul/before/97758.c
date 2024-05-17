xmlXPathParseLiteral(xmlXPathParserContextPtr ctxt) {
    const xmlChar *q;
    xmlChar *ret = NULL;

    if (CUR == '"') {
        NEXT;
	q = CUR_PTR;
	while ((IS_CHAR_CH(CUR)) && (CUR != '"'))
	    NEXT;
	if (!IS_CHAR_CH(CUR)) {
	    XP_ERRORNULL(XPATH_UNFINISHED_LITERAL_ERROR);
	} else {
	    ret = xmlStrndup(q, CUR_PTR - q);
	    NEXT;
        }
    } else if (CUR == '\'') {
        NEXT;
	q = CUR_PTR;
	while ((IS_CHAR_CH(CUR)) && (CUR != '\''))
	    NEXT;
	if (!IS_CHAR_CH(CUR)) {
	    XP_ERRORNULL(XPATH_UNFINISHED_LITERAL_ERROR);
	} else {
	    ret = xmlStrndup(q, CUR_PTR - q);
	    NEXT;
        }
    } else {
	XP_ERRORNULL(XPATH_START_LITERAL_ERROR);
    }
    return(ret);
}
