xmlXPtrEval(const xmlChar *str, xmlXPathContextPtr ctx) {
    xmlXPathParserContextPtr ctxt;
    xmlXPathObjectPtr res = NULL, tmp;
    xmlXPathObjectPtr init = NULL;
    int stack = 0;

    xmlXPathInit();

    if ((ctx == NULL) || (str == NULL))
	return(NULL);

    ctxt = xmlXPathNewParserContext(str, ctx);
    ctxt->xptr = 1;
    xmlXPtrEvalXPointer(ctxt);

    if ((ctxt->value != NULL) &&
	(ctxt->value->type != XPATH_NODESET) &&
	(ctxt->value->type != XPATH_LOCATIONSET)) {
        xmlXPtrErr(ctxt, XML_XPTR_EVAL_FAILED,
		"xmlXPtrEval: evaluation failed to return a node set\n",
		   NULL);
    } else {
	res = valuePop(ctxt);
    }

    do {
        tmp = valuePop(ctxt);
	if (tmp != NULL) {
	    if (tmp != init) {
		if (tmp->type == XPATH_NODESET) {
		     
		    xmlNodeSetPtr set; 
		    set = tmp->nodesetval;
		    if ((set->nodeNr != 1) ||
			(set->nodeTab[0] != (xmlNodePtr) ctx->doc))
			stack++;
		} else
		    stack++;    
	    }
	    xmlXPathFreeObject(tmp);
        }
    } while (tmp != NULL);
    if (stack != 0) {
        xmlXPtrErr(ctxt, XML_XPTR_EXTRA_OBJECTS,
		   "xmlXPtrEval: object(s) left on the eval stack\n",
		   NULL);
    }
    if (ctxt->error != XPATH_EXPRESSION_OK) {
	xmlXPathFreeObject(res);
	res = NULL;
    }
        
    xmlXPathFreeParserContext(ctxt);
    return(res);
}
