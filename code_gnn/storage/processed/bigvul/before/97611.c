xmlXPathCompOpEvalPredicate(xmlXPathParserContextPtr ctxt,
			    xmlXPathStepOpPtr op,
			    xmlNodeSetPtr set,
			    int contextSize,
			    int hasNsNodes)
{
    if (op->ch1 != -1) {
	xmlXPathCompExprPtr comp = ctxt->comp;
	 
	if (comp->steps[op->ch1].op != XPATH_OP_PREDICATE) {
	     
	}
	contextSize = xmlXPathCompOpEvalPredicate(ctxt,
	    &comp->steps[op->ch1], set, contextSize, hasNsNodes);
	CHECK_ERROR0;
	if (contextSize <= 0)
	    return(0);
    }
    if (op->ch2 != -1) {
	xmlXPathContextPtr xpctxt = ctxt->context;
	xmlNodePtr contextNode, oldContextNode;
	xmlDocPtr oldContextDoc;
	int i, res, contextPos = 0, newContextSize;
	xmlXPathStepOpPtr exprOp;
	xmlXPathObjectPtr contextObj = NULL, exprRes = NULL;

#ifdef LIBXML_XPTR_ENABLED
	 
#endif
	 
	oldContextNode = xpctxt->node;
	oldContextDoc = xpctxt->doc;
	 
	exprOp = &ctxt->comp->steps[op->ch2];
	newContextSize = 0;
	for (i = 0; i < set->nodeNr; i++) {
	    if (set->nodeTab[i] == NULL)
		continue;

	    contextNode = set->nodeTab[i];
	    xpctxt->node = contextNode;
	    xpctxt->contextSize = contextSize;
	    xpctxt->proximityPosition = ++contextPos;

	     
	    if ((contextNode->type != XML_NAMESPACE_DECL) &&
		(contextNode->doc != NULL))
		xpctxt->doc = contextNode->doc;
	     
	    if (contextObj == NULL)
		contextObj = xmlXPathCacheNewNodeSet(xpctxt, contextNode);
	    else
		xmlXPathNodeSetAddUnique(contextObj->nodesetval,
		    contextNode);

	    valuePush(ctxt, contextObj);

	    res = xmlXPathCompOpEvalToBoolean(ctxt, exprOp, 1);

	    if ((ctxt->error != XPATH_EXPRESSION_OK) || (res == -1)) {
		xmlXPathNodeSetClear(set, hasNsNodes);
		newContextSize = 0;
		goto evaluation_exit;
	    }

	    if (res != 0) {
		newContextSize++;
	    } else {
		 
		set->nodeTab[i] = NULL;
		if (contextNode->type == XML_NAMESPACE_DECL)
		    xmlXPathNodeSetFreeNs((xmlNsPtr) contextNode);
	    }
	    if (ctxt->value == contextObj) {
		 
		valuePop(ctxt);
		xmlXPathNodeSetClear(contextObj->nodesetval, hasNsNodes);
	    } else {
		 
		contextObj = NULL;
	    }
	}

	if (contextObj != NULL) {
	    if (ctxt->value == contextObj)
		valuePop(ctxt);
	    xmlXPathReleaseObject(xpctxt, contextObj);
	}
evaluation_exit:
	if (exprRes != NULL)
	    xmlXPathReleaseObject(ctxt->context, exprRes);
	 
	xpctxt->node = oldContextNode;
	xpctxt->doc = oldContextDoc;
	xpctxt->contextSize = -1;
	xpctxt->proximityPosition = -1;
	return(newContextSize);
    }
    return(contextSize);
}
