exsltStrEncodeUriFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    int escape_all = 1, str_len = 0;
    xmlChar *str = NULL, *ret = NULL, *tmp;

    if ((nargs < 2) || (nargs > 3)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs >= 3) {
         
        tmp = xmlXPathPopString(ctxt);
        if (xmlUTF8Strlen(tmp) != 5 || xmlStrcmp((const xmlChar *)"UTF-8",tmp)) {
	    xmlXPathReturnEmptyString(ctxt);
	    xmlFree(tmp);
	    return;
	}
	xmlFree(tmp);
    }

    escape_all = xmlXPathPopBoolean(ctxt);

    str = xmlXPathPopString(ctxt);
    str_len = xmlUTF8Strlen(str);

    if (str_len == 0) {
	xmlXPathReturnEmptyString(ctxt);
	xmlFree(str);
	return;
    }

    ret = xmlURIEscapeStr(str,(const xmlChar *)(escape_all?"-_.!~*'()":"-_.!~*'();/?:@&=+$,[]"));
    xmlXPathReturnString(ctxt, ret);

    if (str != NULL)
	xmlFree(str);
}
