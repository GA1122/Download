xmlXPathSubstringFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr str, start, len;
    double le=0, in;
    int i, l, m;
    xmlChar *ret;

    if (nargs < 2) {
	CHECK_ARITY(2);
    }
    if (nargs > 3) {
	CHECK_ARITY(3);
    }
     
    if (nargs == 3) {
	CAST_TO_NUMBER;
	CHECK_TYPE(XPATH_NUMBER);
	len = valuePop(ctxt);
	le = len->floatval;
	xmlXPathReleaseObject(ctxt->context, len);
    }

    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);
    start = valuePop(ctxt);
    in = start->floatval;
    xmlXPathReleaseObject(ctxt->context, start);
    CAST_TO_STRING;
    CHECK_TYPE(XPATH_STRING);
    str = valuePop(ctxt);
    m = xmlUTF8Strlen((const unsigned char *)str->stringval);

     
    if (nargs != 3) {
	le = (double)m;
	if (in < 1.0)
	    in = 1.0;
    }

     
    if (!xmlXPathIsInf(in) && !xmlXPathIsNaN(in + le)) {
         
        i = (int) in;
        if (((double)i)+0.5 <= in) i++;

	if (xmlXPathIsInf(le) == 1) {
	    l = m;
	    if (i < 1)
		i = 1;
	}
	else if (xmlXPathIsInf(le) == -1 || le < 0.0)
	    l = 0;
	else {
	    l = (int) le;
	    if (((double)l)+0.5 <= le) l++;
	}

	 
        i -= 1;
        l += i;
        if (i < 0)
            i = 0;
        if (l > m)
            l = m;

         
        l -= i;

        ret = xmlUTF8Strsub(str->stringval, i, l);
    }
    else {
        ret = NULL;
    }
    if (ret == NULL)
	valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
    else {
	valuePush(ctxt, xmlXPathCacheNewString(ctxt->context, ret));
	xmlFree(ret);
    }
    xmlXPathReleaseObject(ctxt->context, str);
}
