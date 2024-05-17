exsltDateAddDurationFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *ret, *xstr, *ystr;

    if (nargs != 2) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ystr = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    xstr = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt)) {
        xmlFree(ystr);
	return;
    }

    ret = exsltDateAddDuration(xstr, ystr);

    xmlFree(ystr);
    xmlFree(xstr);

    if (ret == NULL)
        xmlXPathReturnEmptyString(ctxt);
    else
	xmlXPathReturnString(ctxt, ret);
}
