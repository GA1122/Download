exsltStrDecodeUriFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    int str_len = 0;
    xmlChar *str = NULL, *ret = NULL, *tmp;

    if ((nargs < 1) || (nargs > 2)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs >= 2) {
         
        tmp = xmlXPathPopString(ctxt);
        if (xmlUTF8Strlen(tmp) != 5 || xmlStrcmp((const xmlChar *)"UTF-8",tmp)) {
	    xmlXPathReturnEmptyString(ctxt);
	    xmlFree(tmp);
	    return;
	}
	xmlFree(tmp);
    }

    str = xmlXPathPopString(ctxt);
    str_len = xmlUTF8Strlen(str);

    if (str_len == 0) {
	xmlXPathReturnEmptyString(ctxt);
	xmlFree(str);
	return;
    }

    ret = (xmlChar *) xmlURIUnescapeString((const char *)str,0,NULL);
    if (!xmlCheckUTF8(ret)) {
	 
	xmlXPathReturnEmptyString(ctxt);
	xmlFree(str);
	xmlFree(ret);
	return;
    }

    xmlXPathReturnString(ctxt, ret);

    if (str != NULL)
	xmlFree(str);
}
