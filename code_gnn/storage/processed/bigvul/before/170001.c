xsltLocalVariablePush(xsltTransformContextPtr ctxt,
		      xsltStackElemPtr variable,
		      int level)
{
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
    ctxt->varsTab[ctxt->varsNr++] = variable;
    ctxt->vars = variable;
    variable->level = level;
    return(0);
}
