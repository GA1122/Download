exsltDateTimeFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *ret, *dt = NULL;

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

    ret = exsltDateTime(dt);

    if (ret == NULL) {
	xsltGenericDebug(xsltGenericDebugContext,
			 "{http://exslt.org/dates-and-times}time: "
			 "invalid date or format %s\n", dt);
	xmlXPathReturnEmptyString(ctxt);
    } else {
	xmlXPathReturnString(ctxt, ret);
    }

    if (dt != NULL)
	xmlFree(dt);
}
