exsltDateLeapYearFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *dt = NULL;
    xmlXPathObjectPtr ret;

    if ((nargs < 0) || (nargs > 1)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 1) {
	dt = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateLeapYear(dt);

    if (dt != NULL)
	xmlFree(dt);

    valuePush(ctxt, ret);
}
