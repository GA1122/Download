valuePush(xmlXPathParserContextPtr ctxt, xmlXPathObjectPtr value)
{
    if ((ctxt == NULL) || (value == NULL)) return(-1);
    if (ctxt->valueNr >= ctxt->valueMax) {
        xmlXPathObjectPtr *tmp;

        tmp = (xmlXPathObjectPtr *) xmlRealloc(ctxt->valueTab,
                                             2 * ctxt->valueMax *
                                             sizeof(ctxt->valueTab[0]));
        if (tmp == NULL) {
            xmlGenericError(xmlGenericErrorContext, "realloc failed !\n");
            return (0);
        }
        ctxt->valueMax *= 2;
	ctxt->valueTab = tmp;
    }
    ctxt->valueTab[ctxt->valueNr] = value;
    ctxt->value = value;
    return (ctxt->valueNr++);
}
