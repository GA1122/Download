xmlXPathCompOpEvalFilterFirst(xmlXPathParserContextPtr ctxt,
			      xmlXPathStepOpPtr op, xmlNodePtr * first)
{
    int total = 0;
    xmlXPathCompExprPtr comp;
    xmlXPathObjectPtr res;
    xmlXPathObjectPtr obj;
    xmlNodeSetPtr oldset;
    xmlNodePtr oldnode;
    xmlDocPtr oldDoc;
    int i;

    CHECK_ERROR0;
    comp = ctxt->comp;
     
    if ((op->ch1 != -1) && (op->ch2 != -1) &&
	(comp->steps[op->ch1].op == XPATH_OP_SORT) &&
	(comp->steps[op->ch2].op == XPATH_OP_SORT)) {
	int f = comp->steps[op->ch2].ch1;

	if ((f != -1) &&
	    (comp->steps[f].op == XPATH_OP_FUNCTION) &&
	    (comp->steps[f].value5 == NULL) &&
	    (comp->steps[f].value == 0) &&
	    (comp->steps[f].value4 != NULL) &&
	    (xmlStrEqual
	    (comp->steps[f].value4, BAD_CAST "last"))) {
	    xmlNodePtr last = NULL;

	    total +=
		xmlXPathCompOpEvalLast(ctxt,
		    &comp->steps[op->ch1],
		    &last);
	    CHECK_ERROR0;
	     
	    if ((ctxt->value != NULL) &&
		(ctxt->value->type == XPATH_NODESET) &&
		(ctxt->value->nodesetval != NULL) &&
		(ctxt->value->nodesetval->nodeTab != NULL) &&
		(ctxt->value->nodesetval->nodeNr > 1)) {
		ctxt->value->nodesetval->nodeTab[0] =
		    ctxt->value->nodesetval->nodeTab[ctxt->
		    value->
		    nodesetval->
		    nodeNr -
		    1];
		ctxt->value->nodesetval->nodeNr = 1;
		*first = *(ctxt->value->nodesetval->nodeTab);
	    }
	    return (total);
	}
    }

    if (op->ch1 != -1)
	total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
    CHECK_ERROR0;
    if (op->ch2 == -1)
	return (total);
    if (ctxt->value == NULL)
	return (total);

#ifdef LIBXML_XPTR_ENABLED
    oldnode = ctxt->context->node;
     
    if (ctxt->value->type == XPATH_LOCATIONSET) {
	xmlXPathObjectPtr tmp = NULL;
	xmlLocationSetPtr newlocset = NULL;
	xmlLocationSetPtr oldlocset;

	 
	CHECK_TYPE0(XPATH_LOCATIONSET);
	obj = valuePop(ctxt);
	oldlocset = obj->user;
	ctxt->context->node = NULL;

	if ((oldlocset == NULL) || (oldlocset->locNr == 0)) {
	    ctxt->context->contextSize = 0;
	    ctxt->context->proximityPosition = 0;
	    if (op->ch2 != -1)
		total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    res = valuePop(ctxt);
	    if (res != NULL) {
		xmlXPathReleaseObject(ctxt->context, res);
	    }
	    valuePush(ctxt, obj);
	    CHECK_ERROR0;
	    return (total);
	}
	newlocset = xmlXPtrLocationSetCreate(NULL);

	for (i = 0; i < oldlocset->locNr; i++) {
	     
	    ctxt->context->node = oldlocset->locTab[i]->user;
	    ctxt->context->contextSize = oldlocset->locNr;
	    ctxt->context->proximityPosition = i + 1;
	    if (tmp == NULL) {
		tmp = xmlXPathCacheNewNodeSet(ctxt->context,
		    ctxt->context->node);
	    } else {
		xmlXPathNodeSetAddUnique(tmp->nodesetval,
		    ctxt->context->node);
	    }
	    valuePush(ctxt, tmp);
	    if (op->ch2 != -1)
		total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    if (ctxt->error != XPATH_EXPRESSION_OK) {
		xmlXPathFreeObject(obj);
		return(0);
	    }
	     
	    res = valuePop(ctxt);
	    if (xmlXPathEvaluatePredicateResult(ctxt, res)) {
		xmlXPtrLocationSetAdd(newlocset,
		    xmlXPathCacheObjectCopy(ctxt->context,
			oldlocset->locTab[i]));
	    }
	     
	    if (res != NULL) {
		xmlXPathReleaseObject(ctxt->context, res);
	    }
	    if (ctxt->value == tmp) {
		valuePop(ctxt);
		xmlXPathNodeSetClear(tmp->nodesetval, 1);
		 
		 
	    } else
		tmp = NULL;
	    ctxt->context->node = NULL;
	     
	    if (newlocset->locNr > 0) {
		*first = (xmlNodePtr) oldlocset->locTab[i]->user;
		break;
	    }
	}
	if (tmp != NULL) {
	    xmlXPathReleaseObject(ctxt->context, tmp);
	}
	 
	xmlXPathReleaseObject(ctxt->context, obj);
	ctxt->context->node = NULL;
	ctxt->context->contextSize = -1;
	ctxt->context->proximityPosition = -1;
	valuePush(ctxt, xmlXPtrWrapLocationSet(newlocset));
	ctxt->context->node = oldnode;
	return (total);
    }
#endif  

     
    CHECK_TYPE0(XPATH_NODESET);
    obj = valuePop(ctxt);
    oldset = obj->nodesetval;

    oldnode = ctxt->context->node;
    oldDoc = ctxt->context->doc;
    ctxt->context->node = NULL;

    if ((oldset == NULL) || (oldset->nodeNr == 0)) {
	ctxt->context->contextSize = 0;
	ctxt->context->proximityPosition = 0;
	 
	valuePush(ctxt, obj);
	ctxt->context->node = oldnode;
	CHECK_ERROR0;
    } else {
	xmlNodeSetPtr newset;
	xmlXPathObjectPtr tmp = NULL;
	 
	newset = xmlXPathNodeSetCreate(NULL);
         

	for (i = 0; i < oldset->nodeNr; i++) {
	     
	    ctxt->context->node = oldset->nodeTab[i];
	    if ((oldset->nodeTab[i]->type != XML_NAMESPACE_DECL) &&
		(oldset->nodeTab[i]->doc != NULL))
		ctxt->context->doc = oldset->nodeTab[i]->doc;
	    if (tmp == NULL) {
		tmp = xmlXPathCacheNewNodeSet(ctxt->context,
		    ctxt->context->node);
	    } else {
		xmlXPathNodeSetAddUnique(tmp->nodesetval,
		    ctxt->context->node);
	    }
	    valuePush(ctxt, tmp);
	    ctxt->context->contextSize = oldset->nodeNr;
	    ctxt->context->proximityPosition = i + 1;
	    if (op->ch2 != -1)
		total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    if (ctxt->error != XPATH_EXPRESSION_OK) {
		xmlXPathFreeNodeSet(newset);
		xmlXPathFreeObject(obj);
		return(0);
	    }
	     
	    res = valuePop(ctxt);
	    if (xmlXPathEvaluatePredicateResult(ctxt, res)) {
		xmlXPathNodeSetAdd(newset, oldset->nodeTab[i]);
	    }
	     
	    if (res != NULL) {
		xmlXPathReleaseObject(ctxt->context, res);
	    }
	    if (ctxt->value == tmp) {
		valuePop(ctxt);
		 
		xmlXPathNodeSetClear(tmp->nodesetval, 1);
	    } else
		tmp = NULL;
	    ctxt->context->node = NULL;
	     
	    if (newset->nodeNr > 0) {
		*first = *(newset->nodeTab);
		break;
	    }
	}
	if (tmp != NULL) {
	    xmlXPathReleaseObject(ctxt->context, tmp);
	}
	 
	xmlXPathReleaseObject(ctxt->context, obj);
	ctxt->context->node = NULL;
	ctxt->context->contextSize = -1;
	ctxt->context->proximityPosition = -1;
	 
	ctxt->context->doc = oldDoc;
	valuePush(ctxt, xmlXPathCacheWrapNodeSet(ctxt->context, newset));
    }
