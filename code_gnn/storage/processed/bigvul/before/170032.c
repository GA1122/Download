xsltGlobalVariableLookup(xsltTransformContextPtr ctxt, const xmlChar *name,
		         const xmlChar *ns_uri) {
    xsltStackElemPtr elem;
    xmlXPathObjectPtr ret = NULL;

     
    if ((ctxt->xpathCtxt == NULL) || (ctxt->globalVars == NULL))
	return(NULL);
    elem = (xsltStackElemPtr)
	    xmlHashLookup2(ctxt->globalVars, name, ns_uri);
    if (elem == NULL) {
#ifdef WITH_XSLT_DEBUG_VARIABLE
	XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
			 "global variable not found %s\n", name));
#endif
	return(NULL);
    }
     
    if (elem->computed == 0) {
	if (elem->name == xsltComputingGlobalVarMarker) {
	    xsltTransformError(ctxt, NULL, elem->comp->inst,
		"Recursive definition of %s\n", name);
	    return(NULL);
	}
	ret = xsltEvalGlobalVariable(elem, ctxt);
    } else
	ret = elem->value;
    return(xmlXPathObjectCopy(ret));
}
