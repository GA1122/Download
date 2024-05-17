exsltDateMonthNameFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *dt = NULL;
    const xmlChar *ret;

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

    ret = exsltDateMonthName(dt);

    if (dt != NULL)
	xmlFree(dt);

    if (ret == NULL)
	xmlXPathReturnEmptyString(ctxt);
    else
	xmlXPathReturnString(ctxt, xmlStrdup(ret));
}
