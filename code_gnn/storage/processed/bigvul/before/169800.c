exsltDateSecondsFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *str = NULL;
    double   ret;

    if (nargs > 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 1) {
	str = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateSeconds(str);
    if (str != NULL)
	xmlFree(str);

    xmlXPathReturnNumber(ctxt, ret);
}
