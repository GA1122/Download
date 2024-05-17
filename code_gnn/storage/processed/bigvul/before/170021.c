xsltEvalGlobalVariable(xsltStackElemPtr elem, xsltTransformContextPtr ctxt)
{
    xmlXPathObjectPtr result = NULL;
    xmlNodePtr oldInst;
    const xmlChar* oldVarName;

#ifdef XSLT_REFACTORED
    xsltStyleBasicItemVariablePtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((ctxt == NULL) || (elem == NULL))
	return(NULL);
    if (elem->computed)
	return(elem->value);


#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	"Evaluating global variable %s\n", elem->name));
#endif

#ifdef WITH_DEBUGGER
    if ((ctxt->debugStatus != XSLT_DEBUG_NONE) &&
        elem->comp && elem->comp->inst)
        xslHandleDebugger(elem->comp->inst, NULL, NULL, ctxt);
#endif

    oldInst = ctxt->inst;
#ifdef XSLT_REFACTORED
    comp = (xsltStyleBasicItemVariablePtr) elem->comp;
#else
    comp = elem->comp;
#endif
    oldVarName = elem->name;
    elem->name = xsltComputingGlobalVarMarker;
     
    if (elem->select != NULL) {
	xmlXPathCompExprPtr xpExpr = NULL;
	xmlDocPtr oldXPDoc;
	xmlNodePtr oldXPContextNode;
	int oldXPProximityPosition, oldXPContextSize, oldXPNsNr;
	xmlNsPtr *oldXPNamespaces;
	xmlXPathContextPtr xpctxt = ctxt->xpathCtxt;

	if ((comp != NULL) && (comp->comp != NULL)) {
	    xpExpr = comp->comp;
	} else {
	    xpExpr = xmlXPathCompile(elem->select);
	}
	if (xpExpr == NULL)
	    goto error;


	if (comp != NULL)
	    ctxt->inst = comp->inst;
	else
	    ctxt->inst = NULL;
	 
	 
	oldXPDoc = xpctxt->doc;
	oldXPContextNode = xpctxt->node;
	oldXPProximityPosition = xpctxt->proximityPosition;
	oldXPContextSize = xpctxt->contextSize;
	oldXPNamespaces = xpctxt->namespaces;
	oldXPNsNr = xpctxt->nsNr;

	xpctxt->node = ctxt->initialContextNode;
	xpctxt->doc = ctxt->initialContextDoc;
	xpctxt->contextSize = 1;
	xpctxt->proximityPosition = 1;

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

	result = xmlXPathCompiledEval(xpExpr, xpctxt);

	 
	xpctxt->doc = oldXPDoc;
	xpctxt->node = oldXPContextNode;
	xpctxt->contextSize = oldXPContextSize;
	xpctxt->proximityPosition = oldXPProximityPosition;
	xpctxt->namespaces = oldXPNamespaces;
	xpctxt->nsNr = oldXPNsNr;

	if ((comp == NULL) || (comp->comp == NULL))
	    xmlXPathFreeCompExpr(xpExpr);
	if (result == NULL) {
	    if (comp == NULL)
		xsltTransformError(ctxt, NULL, NULL,
		    "Evaluating global variable %s failed\n", elem->name);
	    else
		xsltTransformError(ctxt, NULL, comp->inst,
		    "Evaluating global variable %s failed\n", elem->name);
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
	if (elem->tree == NULL) {
	    result = xmlXPathNewCString("");
	} else {
	    xmlDocPtr container;
	    xmlNodePtr oldInsert;
	    xmlDocPtr  oldOutput, oldXPDoc;
	     
	    container = xsltCreateRVT(ctxt);
	    if (container == NULL)
		goto error;
	     
	    xsltRegisterPersistRVT(ctxt, container);

	    oldOutput = ctxt->output;
	    oldInsert = ctxt->insert;

	    oldXPDoc = ctxt->xpathCtxt->doc;

	    ctxt->output = container;
	    ctxt->insert = (xmlNodePtr) container;

	    ctxt->xpathCtxt->doc = ctxt->initialContextDoc;
	     
	    xsltApplyOneTemplate(ctxt, ctxt->node, elem->tree, NULL, NULL);

	    ctxt->xpathCtxt->doc = oldXPDoc;

	    ctxt->insert = oldInsert;
	    ctxt->output = oldOutput;

	    result = xmlXPathNewValueTree((xmlNodePtr) container);
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
    elem->name = oldVarName;
    ctxt->inst = oldInst;
    if (result != NULL) {
	elem->value = result;
	elem->computed = 1;
    }
    return(result);
}
