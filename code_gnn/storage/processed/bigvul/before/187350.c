 xsltIf(xsltTransformContextPtr ctxt, xmlNodePtr contextNode,
 	           xmlNodePtr inst, xsltStylePreCompPtr castedComp)
 {
     int res = 0;
 
 #ifdef XSLT_REFACTORED
     xsltStyleItemIfPtr comp = (xsltStyleItemIfPtr) castedComp;
 #else
     xsltStylePreCompPtr comp = castedComp;
 #endif
 
     if ((ctxt == NULL) || (contextNode == NULL) || (inst == NULL))
 	return;
     if ((comp == NULL) || (comp->test == NULL) || (comp->comp == NULL)) {
 	xsltTransformError(ctxt, NULL, inst,
 	    "Internal error in xsltIf(): "
 	    "The XSLT 'if' instruction was not compiled.\n");
 	return;
     }
 
 #ifdef WITH_XSLT_DEBUG_PROCESS
     XSLT_TRACE(ctxt,XSLT_TRACE_IF,xsltGenericDebug(xsltGenericDebugContext,
 	 "xsltIf: test %s\n", comp->test));
 #endif
  
  #ifdef XSLT_FAST_IF
      {
	xmlXPathContextPtr xpctxt = ctxt->xpathCtxt;
	xmlDocPtr oldXPContextDoc = xpctxt->doc;
	xmlNsPtr *oldXPNamespaces = xpctxt->namespaces;
	xmlNodePtr oldXPContextNode = xpctxt->node;
	int oldXPProximityPosition = xpctxt->proximityPosition;
	int oldXPContextSize = xpctxt->contextSize;
	int oldXPNsNr = xpctxt->nsNr;
  	xmlDocPtr oldLocalFragmentTop = ctxt->localRVT;
  
	xpctxt->node = contextNode;
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
	 
	res = xmlXPathCompiledEvalToBoolean(comp->comp, xpctxt);
// 	res = xsltPreCompEvalToBoolean(ctxt, contextNode, comp);
  
  	 
  	if (oldLocalFragmentTop != ctxt->localRVT)
  	    xsltReleaseLocalRVTs(ctxt, oldLocalFragmentTop);
	xpctxt->doc = oldXPContextDoc;
	xpctxt->node = oldXPContextNode;
	xpctxt->contextSize = oldXPContextSize;
	xpctxt->proximityPosition = oldXPProximityPosition;
	xpctxt->nsNr = oldXPNsNr;
	xpctxt->namespaces = oldXPNamespaces;
      }
  
  #ifdef WITH_XSLT_DEBUG_PROCESS
     XSLT_TRACE(ctxt,XSLT_TRACE_IF,xsltGenericDebug(xsltGenericDebugContext,
 	"xsltIf: test evaluate to %d\n", res));
 #endif
 
     if (res == -1) {
 	ctxt->state = XSLT_STATE_STOPPED;
 	goto error;
     }
     if (res == 1) {
 	 
 	xsltApplySequenceConstructor(ctxt,
 	    contextNode, inst->children, NULL);
     }
  
  #else  
      {
	xmlXPathObjectPtr xpobj = NULL;
  	 
	{
	    xmlXPathContextPtr xpctxt = ctxt->xpathCtxt;
	    xmlDocPtr oldXPContextDoc = xpctxt->doc;
	    xmlNsPtr *oldXPNamespaces = xpctxt->namespaces;
	    xmlNodePtr oldXPContextNode = xpctxt->node;
	    int oldXPProximityPosition = xpctxt->proximityPosition;
	    int oldXPContextSize = xpctxt->contextSize;
	    int oldXPNsNr = xpctxt->nsNr;
	    xpctxt->node = contextNode;
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
	     
	    xpobj = xmlXPathCompiledEval(comp->comp, xpctxt);
	    xpctxt->doc = oldXPContextDoc;
	    xpctxt->node = oldXPContextNode;
	    xpctxt->contextSize = oldXPContextSize;
	    xpctxt->proximityPosition = oldXPProximityPosition;
	    xpctxt->nsNr = oldXPNsNr;
	    xpctxt->namespaces = oldXPNamespaces;
	}
// 	xmlXPathObjectPtr xpobj = xsltPreCompEval(ctxt, contextNode, comp);
  	if (xpobj != NULL) {
  	    if (xpobj->type != XPATH_BOOLEAN)
  		xpobj = xmlXPathConvertBoolean(xpobj);
 	    if (xpobj->type == XPATH_BOOLEAN) {
 		res = xpobj->boolval;
 
 #ifdef WITH_XSLT_DEBUG_PROCESS
 		XSLT_TRACE(ctxt,XSLT_TRACE_IF,xsltGenericDebug(xsltGenericDebugContext,
 		    "xsltIf: test evaluate to %d\n", res));
 #endif
 		if (res) {
 		    xsltApplySequenceConstructor(ctxt,
 			contextNode, inst->children, NULL);
 		}
 	    } else {
 
 #ifdef WITH_XSLT_DEBUG_PROCESS
 		XSLT_TRACE(ctxt, XSLT_TRACE_IF,
 		    xsltGenericDebug(xsltGenericDebugContext,
 		    "xsltIf: test didn't evaluate to a boolean\n"));
 #endif
 		ctxt->state = XSLT_STATE_STOPPED;
 	    }
 	    xmlXPathFreeObject(xpobj);
 	} else {
 	    ctxt->state = XSLT_STATE_STOPPED;
 	}
     }
 #endif  
 
 error:
     return;
 }