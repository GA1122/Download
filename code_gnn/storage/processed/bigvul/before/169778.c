exsltDateDurationFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *ret;
    xmlChar *number = NULL;

    if ((nargs < 0) || (nargs > 1)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 1) {
	number = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateDuration(number);

    if (number != NULL)
	xmlFree(number);

    if (ret == NULL)
	xmlXPathReturnEmptyString(ctxt);
    else
	xmlXPathReturnString(ctxt, ret);
}
