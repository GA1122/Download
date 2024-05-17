xmlXPathCompOpEvalToBoolean(xmlXPathParserContextPtr ctxt,
			    xmlXPathStepOpPtr op,
			    int isPredicate)
{
    xmlXPathObjectPtr resObj = NULL;

start:
     
    switch (op->op) {
        case XPATH_OP_END:
            return (0);
	case XPATH_OP_VALUE:
	    resObj = (xmlXPathObjectPtr) op->value4;
	    if (isPredicate)
		return(xmlXPathEvaluatePredicateResult(ctxt, resObj));
	    return(xmlXPathCastToBoolean(resObj));
	case XPATH_OP_SORT:
	     
            if (op->ch1 != -1) {
		op = &ctxt->comp->steps[op->ch1];
		goto start;
	    }
	    return(0);
	case XPATH_OP_COLLECT:
	    if (op->ch1 == -1)
		return(0);

            xmlXPathCompOpEval(ctxt, &ctxt->comp->steps[op->ch1]);
	    if (ctxt->error != XPATH_EXPRESSION_OK)
		return(-1);

            xmlXPathNodeCollectAndTest(ctxt, op, NULL, NULL, 1);
	    if (ctxt->error != XPATH_EXPRESSION_OK)
		return(-1);

	    resObj = valuePop(ctxt);
	    if (resObj == NULL)
		return(-1);
	    break;
	default:
	     
	    xmlXPathCompOpEval(ctxt, op);
	    if (ctxt->error != XPATH_EXPRESSION_OK)
		return(-1);

	    resObj = valuePop(ctxt);
	    if (resObj == NULL)
		return(-1);
	    break;
    }

    if (resObj) {
	int res;

	if (resObj->type == XPATH_BOOLEAN) {
	    res = resObj->boolval;
	} else if (isPredicate) {
	     
	    res = xmlXPathEvaluatePredicateResult(ctxt, resObj);
	} else {
	    res = xmlXPathCastToBoolean(resObj);
	}
	xmlXPathReleaseObject(ctxt->context, resObj);
	return(res);
    }

    return(0);
}
