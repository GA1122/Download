xsltEvalGlobalVariables(xsltTransformContextPtr ctxt) {
    xsltStackElemPtr elem;
    xsltStylesheetPtr style;

    if ((ctxt == NULL) || (ctxt->document == NULL))
	return(-1);

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	"Registering global variables\n"));
#endif
     
    style = ctxt->style;
    while (style != NULL) {
	elem = style->variables;

#ifdef WITH_XSLT_DEBUG_VARIABLE
	if ((style->doc != NULL) && (style->doc->URL != NULL)) {
	    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
			     "Registering global variables from %s\n",
		             style->doc->URL));
	}
#endif

	while (elem != NULL) {
	    xsltStackElemPtr def;

	     
	    def = (xsltStackElemPtr)
		    xmlHashLookup2(ctxt->globalVars,
		                 elem->name, elem->nameURI);
	    if (def == NULL) {

		def = xsltCopyStackElem(elem);
		xmlHashAddEntry2(ctxt->globalVars,
				 elem->name, elem->nameURI, def);
	    } else if ((elem->comp != NULL) &&
		       (elem->comp->type == XSLT_FUNC_VARIABLE)) {
		 
		if ((elem->comp->inst != NULL) &&
		    (def->comp != NULL) && (def->comp->inst != NULL) &&
		    (elem->comp->inst->doc == def->comp->inst->doc))
		{
		    xsltTransformError(ctxt, style, elem->comp->inst,
			"Global variable %s already defined\n", elem->name);
		    if (style != NULL) style->errors++;
		}
	    }
	    elem = elem->next;
	}

	style = xsltNextImport(style);
    }

     
    xmlHashScan(ctxt->globalVars,
	        (xmlHashScanner) xsltEvalGlobalVariable, ctxt);

    return(0);
}
