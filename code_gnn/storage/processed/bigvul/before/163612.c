htmlParseAttValue(htmlParserCtxtPtr ctxt) {
    xmlChar *ret = NULL;

    if (CUR == '"') {
        NEXT;
	ret = htmlParseHTMLAttribute(ctxt, '"');
        if (CUR != '"') {
	    htmlParseErr(ctxt, XML_ERR_ATTRIBUTE_NOT_FINISHED,
	                 "AttValue: \" expected\n", NULL, NULL);
	} else
	    NEXT;
    } else if (CUR == '\'') {
        NEXT;
	ret = htmlParseHTMLAttribute(ctxt, '\'');
        if (CUR != '\'') {
	    htmlParseErr(ctxt, XML_ERR_ATTRIBUTE_NOT_FINISHED,
	                 "AttValue: ' expected\n", NULL, NULL);
	} else
	    NEXT;
    } else {
         
	ret = htmlParseHTMLAttribute(ctxt, 0);
	if (ret == NULL) {
	    htmlParseErr(ctxt, XML_ERR_ATTRIBUTE_WITHOUT_VALUE,
	                 "AttValue: no value found\n", NULL, NULL);
	}
    }
    return(ret);
}
