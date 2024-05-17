xmlXPtrRangeToFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr range;
    const xmlChar *cur;
    xmlXPathObjectPtr res, obj;
    xmlXPathObjectPtr tmp;
    xmlLocationSetPtr newset = NULL;
    xmlNodeSetPtr oldset;
    int i;

    if (ctxt == NULL) return;
    CHECK_ARITY(1);
     
    CHECK_TYPE(XPATH_NODESET);
    obj = valuePop(ctxt);
    oldset = obj->nodesetval;
    ctxt->context->node = NULL;

    cur = ctxt->cur;
    newset = xmlXPtrLocationSetCreate(NULL);
    
    for (i = 0; i < oldset->nodeNr; i++) {
	ctxt->cur = cur;

	 
	ctxt->context->node = oldset->nodeTab[i];
	tmp = xmlXPathNewNodeSet(ctxt->context->node);
	valuePush(ctxt, tmp);

	xmlXPathEvalExpr(ctxt);
	CHECK_ERROR;

	 
	res = valuePop(ctxt);
	range = xmlXPtrNewRangeNodeObject(oldset->nodeTab[i], res);
	if (range != NULL) {
	    xmlXPtrLocationSetAdd(newset, range);
	}

	 
	if (res != NULL)
	    xmlXPathFreeObject(res);
	if (ctxt->value == tmp) {
	    res = valuePop(ctxt);
	    xmlXPathFreeObject(res);
	}
	
	ctxt->context->node = NULL;
    }

     
    xmlXPathFreeObject(obj);
    ctxt->context->node = NULL;
    ctxt->context->contextSize = -1;
    ctxt->context->proximityPosition = -1;
    valuePush(ctxt, xmlXPtrWrapLocationSet(newset));
}
