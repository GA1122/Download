xsltAddStackElem(xsltTransformContextPtr ctxt, xsltStackElemPtr elem)
{
    if ((ctxt == NULL) || (elem == NULL))
	return(-1);

    do {
	if (ctxt->varsMax == 0) {
	    ctxt->varsMax = 10;
	    ctxt->varsTab =
		(xsltStackElemPtr *) xmlMalloc(ctxt->varsMax *
		sizeof(ctxt->varsTab[0]));
	    if (ctxt->varsTab == NULL) {
		xmlGenericError(xmlGenericErrorContext, "malloc failed !\n");
		return (-1);
	    }
	}
	if (ctxt->varsNr >= ctxt->varsMax) {
	    ctxt->varsMax *= 2;
	    ctxt->varsTab =
		(xsltStackElemPtr *) xmlRealloc(ctxt->varsTab,
		ctxt->varsMax *
		sizeof(ctxt->varsTab[0]));
	    if (ctxt->varsTab == NULL) {
		xmlGenericError(xmlGenericErrorContext, "realloc failed !\n");
		return (-1);
	    }
	}
	ctxt->varsTab[ctxt->varsNr++] = elem;
	ctxt->vars = elem;

	elem = elem->next;
    } while (elem != NULL);

    return(0);
}
