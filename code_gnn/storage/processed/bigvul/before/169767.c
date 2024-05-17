exsltDateDateTimeFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *ret;

    if (nargs != 0) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    ret = exsltDateDateTime();
    if (ret == NULL)
        xmlXPathReturnEmptyString(ctxt);
    else
        xmlXPathReturnString(ctxt, ret);
}
