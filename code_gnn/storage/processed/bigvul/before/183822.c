 xmlXPathCompOpEvalPositionalPredicate(xmlXPathParserContextPtr ctxt,
 				      xmlXPathStepOpPtr op,
 				      xmlNodeSetPtr set,
 				      int contextSize,
 				      int minPos,
 				      int maxPos,
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
      
     if (contextSize < minPos) {
 	xmlXPathNodeSetClear(set, hasNsNodes);
 	return(0);
     }
     if (op->ch2 == -1) {
 	 
 	return (contextSize);
     } else {
 	xmlDocPtr oldContextDoc;
 	int i, pos = 0, newContextSize = 0, contextPos = 0, res;
 	xmlXPathStepOpPtr exprOp;
 	xmlXPathObjectPtr contextObj = NULL, exprRes = NULL;
 	xmlNodePtr oldContextNode, contextNode = NULL;
 	xmlXPathContextPtr xpctxt = ctxt->context;
 
 #ifdef LIBXML_XPTR_ENABLED
 	     
 #endif  
 
 	 
 	oldContextNode = xpctxt->node;
 	oldContextDoc = xpctxt->doc;
 	 
 	exprOp = &ctxt->comp->steps[op->ch2];
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
  	        xmlXPathObjectPtr tmp;
  		 
  		tmp = valuePop(ctxt);
                if (tmp != contextObj)
//                 while (tmp != contextObj) {
                       
                      xmlXPathReleaseObject(xpctxt, tmp);
                    valuePop(ctxt);
//                     tmp = valuePop(ctxt);
//                 }
  		goto evaluation_error;
  	    }
  
 	    if (res)
 		pos++;
 
 	    if (res && (pos >= minPos) && (pos <= maxPos)) {
 		 
 		newContextSize++;
 		if (minPos == maxPos) {
 		     
 		    if (contextNode->type == XML_NAMESPACE_DECL) {
 			 
 			set->nodeTab[i] = NULL;
 		    }
 		    xmlXPathNodeSetClear(set, hasNsNodes);
 		    set->nodeNr = 1;
 		    set->nodeTab[0] = contextNode;
 		    goto evaluation_exit;
 		}
 		if (pos == maxPos) {
 		     
 		    xmlXPathNodeSetClearFromPos(set, i +1, hasNsNodes);
 		    goto evaluation_exit;
 		}
 	    } else {
 		 
 		set->nodeTab[i] = NULL;
 		if (contextNode->type == XML_NAMESPACE_DECL)
 		    xmlXPathNodeSetFreeNs((xmlNsPtr) contextNode);
 	    }
 	    if (exprRes != NULL) {
 		xmlXPathReleaseObject(ctxt->context, exprRes);
 		exprRes = NULL;
 	    }
 	    if (ctxt->value == contextObj) {
 		 
 		valuePop(ctxt);
 		xmlXPathNodeSetClear(contextObj->nodesetval, hasNsNodes);
 	    } else {
 		 
 		contextObj = NULL;
 	    }
 	}
 	goto evaluation_exit;
 
 evaluation_error:
 	xmlXPathNodeSetClear(set, hasNsNodes);
 	newContextSize = 0;
 
 evaluation_exit:
 	if (contextObj != NULL) {
 	    if (ctxt->value == contextObj)
 		valuePop(ctxt);
 	    xmlXPathReleaseObject(xpctxt, contextObj);
 	}
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