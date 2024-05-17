xsltRegisterTmpRVT(xsltTransformContextPtr ctxt, xmlDocPtr RVT)
{
    if ((ctxt == NULL) || (RVT == NULL))
	return(-1);

     
    if (ctxt->contextVariable != NULL) {
	RVT->next = (xmlNodePtr) XSLT_TCTXT_VARIABLE(ctxt)->fragment;
	XSLT_TCTXT_VARIABLE(ctxt)->fragment = RVT;
	return(0);
    }

    RVT->next = (xmlNodePtr) ctxt->tmpRVT;
    if (ctxt->tmpRVT != NULL)
	ctxt->tmpRVT->prev = (xmlNodePtr) RVT;
    ctxt->tmpRVT = RVT;
    return(0);
}
