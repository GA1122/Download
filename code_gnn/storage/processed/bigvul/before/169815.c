exsltFuncResultElem (xsltTransformContextPtr ctxt,
	             xmlNodePtr node ATTRIBUTE_UNUSED, xmlNodePtr inst,
		     exsltFuncResultPreComp *comp) {
    exsltFuncData *data;
    xmlXPathObjectPtr ret;


     
    data = (exsltFuncData *) xsltGetExtData (ctxt, EXSLT_FUNCTIONS_NAMESPACE);
    if (data == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncReturnElem: data == NULL\n");
	return;
    }
    if (data->result != NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "func:result already instanciated\n");
	data->error = 1;
	return;
    }
     
    if (comp->select != NULL) {
	xmlNsPtr *oldXPNsList;
	int oldXPNsNr;
	xmlNodePtr oldXPContextNode;
	 
	if (inst->children != NULL) {
	    xsltGenericError(xsltGenericErrorContext,
			     "func:result content must be empty if"
			     " the function has a select attribute\n");
	    data->error = 1;
	    return;
	}
	oldXPNsList = ctxt->xpathCtxt->namespaces;
	oldXPNsNr = ctxt->xpathCtxt->nsNr;
	oldXPContextNode = ctxt->xpathCtxt->node;

	ctxt->xpathCtxt->namespaces = comp->nsList;
	ctxt->xpathCtxt->nsNr = comp->nsNr;

	ret = xmlXPathCompiledEval(comp->select, ctxt->xpathCtxt);

	ctxt->xpathCtxt->node = oldXPContextNode;
	ctxt->xpathCtxt->nsNr = oldXPNsNr;
	ctxt->xpathCtxt->namespaces = oldXPNsList;

	if (ret == NULL) {
	    xsltGenericError(xsltGenericErrorContext,
			     "exsltFuncResultElem: ret == NULL\n");
	    return;
	}
	 
	xsltExtensionInstructionResultRegister(ctxt, ret);
    } else if (inst->children != NULL) {
	 
	xmlNodePtr oldInsert;
	xmlDocPtr container;

	container = xsltCreateRVT(ctxt);
	if (container == NULL) {
	    xsltGenericError(xsltGenericErrorContext,
			     "exsltFuncResultElem: out of memory\n");
	    data->error = 1;
	    return;
	}
	xsltRegisterLocalRVT(ctxt, container);

	oldInsert = ctxt->insert;
	ctxt->insert = (xmlNodePtr) container;
	xsltApplyOneTemplate (ctxt, ctxt->xpathCtxt->node,
			      inst->children, NULL, NULL);
	ctxt->insert = oldInsert;

	ret = xmlXPathNewValueTree((xmlNodePtr) container);
	if (ret == NULL) {
	    xsltGenericError(xsltGenericErrorContext,
			     "exsltFuncResultElem: ret == NULL\n");
	    data->error = 1;
	} else {
	    ret->boolval = 0;  
	     
	    xsltExtensionInstructionResultRegister(ctxt, ret);
	}
    } else {
	 
	ret = xmlXPathNewCString("");
    }
    data->result = ret;
}
