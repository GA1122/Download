xsltEvalVariable(xsltTransformContextPtr ctxt, xsltStackElemPtr variable,
	         xsltStylePreCompPtr castedComp)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemVariablePtr comp =
	(xsltStyleItemVariablePtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    xmlXPathObjectPtr result = NULL;
    xmlNodePtr oldInst;

    if ((ctxt == NULL) || (variable == NULL))
	return(NULL);

     
    oldInst = ctxt->inst;

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	"Evaluating variable '%s'\n", variable->name));
#endif
    if (variable->select != NULL) {
	xmlXPathCompExprPtr xpExpr = NULL;
	xmlDocPtr oldXPDoc;
	xmlNodePtr oldXPContextNode;
	int oldXPProximityPosition, oldXPContextSize, oldXPNsNr;
	xmlNsPtr *oldXPNamespaces;
	xmlXPathContextPtr xpctxt = ctxt->xpathCtxt;
	xsltStackElemPtr oldVar = ctxt->contextVariable;

	if ((comp != NULL) && (comp->comp != NULL)) {
	    xpExpr = comp->comp;
	} else {
	    xpExpr = xmlXPathCompile(variable->select);
	}
	if (xpExpr == NULL)
	    return(NULL);
	 
	oldXPDoc = xpctxt->doc;
	oldXPContextNode = xpctxt->node;
	oldXPProximityPosition = xpctxt->proximityPosition;
	oldXPContextSize = xpctxt->contextSize;
	oldXPNamespaces = xpctxt->namespaces;
	oldXPNsNr = xpctxt->nsNr;

	xpctxt->node = ctxt->node;
	 
	if ((ctxt->node->type != XML_NAMESPACE_DECL) &&
	    ctxt->node->doc)
	    xpctxt->doc = ctxt->node->doc;
	 
	if (comp != NULL) {

#ifdef XSLT_REFACTORED
	    if (comp->inScopeNs != NULL) {
		xpctxt->namespaces = comp->inScopeNs->list;
		xpctxt->nsNr = comp->inScopeNs->xpathNumber;
	    } else {
		xpctxt->namespaces = NULL;
		xpctxt->nsNr = 0;
	    }
#else
	    xpctxt->namespaces = comp->nsList;
	    xpctxt->nsNr = comp->nsNr;
#endif
	} else {
	    xpctxt->namespaces = NULL;
	    xpctxt->nsNr = 0;
	}

	 
	ctxt->contextVariable = variable;
	variable->flags |= XSLT_VAR_IN_SELECT;

	result = xmlXPathCompiledEval(xpExpr, xpctxt);

	variable->flags ^= XSLT_VAR_IN_SELECT;
	 
	ctxt->contextVariable = oldVar;

	xpctxt->doc = oldXPDoc;
	xpctxt->node = oldXPContextNode;
	xpctxt->contextSize = oldXPContextSize;
	xpctxt->proximityPosition = oldXPProximityPosition;
	xpctxt->namespaces = oldXPNamespaces;
	xpctxt->nsNr = oldXPNsNr;

	if ((comp == NULL) || (comp->comp == NULL))
	    xmlXPathFreeCompExpr(xpExpr);
	if (result == NULL) {
	    xsltTransformError(ctxt, NULL,
		(comp != NULL) ? comp->inst : NULL,
		"Failed to evaluate the expression of variable '%s'.\n",
		variable->name);
	    ctxt->state = XSLT_STATE_STOPPED;

#ifdef WITH_XSLT_DEBUG_VARIABLE
#ifdef LIBXML_DEBUG_ENABLED
	} else {
	    if ((xsltGenericDebugContext == stdout) ||
		(xsltGenericDebugContext == stderr))
		xmlXPathDebugDumpObject((FILE *)xsltGenericDebugContext,
					result, 0);
#endif
#endif
	}
    } else {
	if (variable->tree == NULL) {
	    result = xmlXPathNewCString("");
	} else {
	    if (variable->tree) {
		xmlDocPtr container;
		xmlNodePtr oldInsert;
		xmlDocPtr  oldOutput;
		xsltStackElemPtr oldVar = ctxt->contextVariable;

		 
		container = xsltCreateRVT(ctxt);
		if (container == NULL)
		    goto error;
		 
		 
		variable->fragment = container;

		oldOutput = ctxt->output;
		oldInsert = ctxt->insert;

		ctxt->output = container;
		ctxt->insert = (xmlNodePtr) container;
		ctxt->contextVariable = variable;
		 
		xsltApplyOneTemplate(ctxt, ctxt->node, variable->tree,
		    NULL, NULL);

		ctxt->contextVariable = oldVar;
		ctxt->insert = oldInsert;
		ctxt->output = oldOutput;

		result = xmlXPathNewValueTree((xmlNodePtr) container);
	    }
	    if (result == NULL) {
		result = xmlXPathNewCString("");
	    } else {
		 
	        result->boolval = 0;
	    }
#ifdef WITH_XSLT_DEBUG_VARIABLE
#ifdef LIBXML_DEBUG_ENABLED

	    if ((xsltGenericDebugContext == stdout) ||
		(xsltGenericDebugContext == stderr))
		xmlXPathDebugDumpObject((FILE *)xsltGenericDebugContext,
					result, 0);
#endif
#endif
	}
    }

error:
    ctxt->inst = oldInst;
    return(result);
}
