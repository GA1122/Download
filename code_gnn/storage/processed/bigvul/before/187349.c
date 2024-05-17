 xsltForEach(xsltTransformContextPtr ctxt, xmlNodePtr contextNode,
 	    xmlNodePtr inst, xsltStylePreCompPtr castedComp)
 {
 #ifdef XSLT_REFACTORED
     xsltStyleItemForEachPtr comp = (xsltStyleItemForEachPtr) castedComp;
 #else
     xsltStylePreCompPtr comp = castedComp;
 #endif
     int i;
     xmlXPathObjectPtr res = NULL;
     xmlNodePtr cur, curInst;
     xmlNodeSetPtr list = NULL;
     xmlNodeSetPtr oldList;
     int oldXPProximityPosition, oldXPContextSize;
     xmlNodePtr oldContextNode;
     xsltTemplatePtr oldCurTemplRule;
     xmlDocPtr oldXPDoc;
     xsltDocumentPtr oldDocInfo;
     xmlXPathContextPtr xpctxt;
 
     if ((ctxt == NULL) || (contextNode == NULL) || (inst == NULL)) {
 	xsltGenericError(xsltGenericErrorContext,
 	    "xsltForEach(): Bad arguments.\n");
 	return;
     }
 
     if (comp == NULL) {
         xsltTransformError(ctxt, NULL, inst,
 	    "Internal error in xsltForEach(): "
 	    "The XSLT 'for-each' instruction was not compiled.\n");
         return;
     }
     if ((comp->select == NULL) || (comp->comp == NULL)) {
 	xsltTransformError(ctxt, NULL, inst,
 	    "Internal error in xsltForEach(): "
 	    "The selecting expression of the XSLT 'for-each' "
 	    "instruction was not compiled correctly.\n");
 	return;
     }
     xpctxt = ctxt->xpathCtxt;
 
 #ifdef WITH_XSLT_DEBUG_PROCESS
     XSLT_TRACE(ctxt,XSLT_TRACE_FOR_EACH,xsltGenericDebug(xsltGenericDebugContext,
 	 "xsltForEach: select %s\n", comp->select));
 #endif
 
      
     oldDocInfo = ctxt->document;
     oldList = ctxt->nodeList;
     oldContextNode = ctxt->node;
      
     oldCurTemplRule = ctxt->currentTemplateRule;
     ctxt->currentTemplateRule = NULL;
 
      oldXPDoc = xpctxt->doc;
      oldXPProximityPosition = xpctxt->proximityPosition;
      oldXPContextSize = xpctxt->contextSize;
     
    xpctxt->node = contextNode;
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
  
       
    res = xmlXPathCompiledEval(comp->comp, ctxt->xpathCtxt);
//     res = xsltPreCompEval(ctxt, contextNode, comp);
  
      if (res != NULL) {
  	if (res->type == XPATH_NODESET)
 	    list = res->nodesetval;
 	else {
 	    xsltTransformError(ctxt, NULL, inst,
 		"The 'select' expression does not evaluate to a node set.\n");
 
 #ifdef WITH_XSLT_DEBUG_PROCESS
 	    XSLT_TRACE(ctxt,XSLT_TRACE_FOR_EACH,xsltGenericDebug(xsltGenericDebugContext,
 		"xsltForEach: select didn't evaluate to a node list\n"));
 #endif
 	    goto error;
 	}
     } else {
 	xsltTransformError(ctxt, NULL, inst,
 	    "Failed to evaluate the 'select' expression.\n");
 	ctxt->state = XSLT_STATE_STOPPED;
 	goto error;
     }
 
     if ((list == NULL) || (list->nodeNr <= 0))
 	goto exit;
 
 #ifdef WITH_XSLT_DEBUG_PROCESS
     XSLT_TRACE(ctxt,XSLT_TRACE_FOR_EACH,xsltGenericDebug(xsltGenericDebugContext,
  	"xsltForEach: select evaluates to %d nodes\n", list->nodeNr));
  #endif
  
     
    xpctxt->contextSize = oldXPContextSize;
    xpctxt->proximityPosition = oldXPProximityPosition;
    xpctxt->node = contextNode;
       
     ctxt->nodeList = list;
      
     curInst = inst->children;
     if (IS_XSLT_ELEM(curInst) && IS_XSLT_NAME(curInst, "sort")) {
 	int nbsorts = 0;
 	xmlNodePtr sorts[XSLT_MAX_SORT];
 
 	sorts[nbsorts++] = curInst;
 
 #ifdef WITH_DEBUGGER
 	if (xslDebugStatus != XSLT_DEBUG_NONE)
 	    xslHandleDebugger(curInst, contextNode, NULL, ctxt);
 #endif
 
 	curInst = curInst->next;
 	while (IS_XSLT_ELEM(curInst) && IS_XSLT_NAME(curInst, "sort")) {
 	    if (nbsorts >= XSLT_MAX_SORT) {
 		xsltTransformError(ctxt, NULL, curInst,
 		    "The number of xsl:sort instructions exceeds the "
 		    "maximum (%d) allowed by this processor.\n",
 		    XSLT_MAX_SORT);
 		goto error;
 	    } else {
 		sorts[nbsorts++] = curInst;
 	    }
 
 #ifdef WITH_DEBUGGER
 	    if (xslDebugStatus != XSLT_DEBUG_NONE)
 		xslHandleDebugger(curInst, contextNode, NULL, ctxt);
 #endif
 	    curInst = curInst->next;
 	}
 	xsltDoSortFunction(ctxt, sorts, nbsorts);
     }
     xpctxt->contextSize = list->nodeNr;
      
     for (i = 0; i < list->nodeNr; i++) {
 	cur = list->nodeTab[i];
 	 
 	ctxt->node = cur;
 	 
 	if ((cur->type != XML_NAMESPACE_DECL) && (cur->doc != NULL))
 	    xpctxt->doc = cur->doc;
 
 	xpctxt->proximityPosition = i + 1;
 
 	xsltApplySequenceConstructor(ctxt, cur, curInst, NULL);
     }
 
 exit:
 error:
     if (res != NULL)
 	xmlXPathFreeObject(res);
      
     ctxt->document = oldDocInfo;
     ctxt->nodeList = oldList;
     ctxt->node = oldContextNode;
     ctxt->currentTemplateRule = oldCurTemplRule;
 
     xpctxt->doc = oldXPDoc;
     xpctxt->contextSize = oldXPContextSize;
     xpctxt->proximityPosition = oldXPProximityPosition;
 }