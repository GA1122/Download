xsltCompilerVarInfoPush(xsltCompilerCtxtPtr cctxt,
				  xmlNodePtr inst,
				  const xmlChar *name,
				  const xmlChar *nsName)
{
    xsltVarInfoPtr ivar;

    if ((cctxt->ivar != NULL) && (cctxt->ivar->next != NULL)) {
	ivar = cctxt->ivar->next;
    } else if ((cctxt->ivar == NULL) && (cctxt->ivars != NULL)) {
	ivar = cctxt->ivars;
    } else {
	ivar = (xsltVarInfoPtr) xmlMalloc(sizeof(xsltVarInfo));
	if (ivar == NULL) {
	    xsltTransformError(NULL, cctxt->style, inst,
		"xsltParseInScopeVarPush: xmlMalloc() failed!\n");
	    cctxt->style->errors++;
	    return(NULL);
	}
	 
	if (cctxt->ivars == NULL) {
	    cctxt->ivars = ivar;
	    ivar->prev = NULL;
	} else {
	    cctxt->ivar->next = ivar;
	    ivar->prev = cctxt->ivar;
	}
	cctxt->ivar = ivar;
	ivar->next = NULL;
    }
    ivar->depth = cctxt->depth;
    ivar->name = name;
    ivar->nsName = nsName;
    return(ivar);
}
