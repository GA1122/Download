xmlXPathEqualValues(xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr arg1, arg2, argtmp;
    int ret = 0;

    if ((ctxt == NULL) || (ctxt->context == NULL)) return(0);
    arg2 = valuePop(ctxt);
    arg1 = valuePop(ctxt);
    if ((arg1 == NULL) || (arg2 == NULL)) {
	if (arg1 != NULL)
	    xmlXPathReleaseObject(ctxt->context, arg1);
	else
	    xmlXPathReleaseObject(ctxt->context, arg2);
	XP_ERROR0(XPATH_INVALID_OPERAND);
    }

    if (arg1 == arg2) {
#ifdef DEBUG_EXPR
        xmlGenericError(xmlGenericErrorContext,
		"Equal: by pointer\n");
#endif
	xmlXPathFreeObject(arg1);
        return(1);
    }

     
    if ((arg2->type == XPATH_NODESET) || (arg2->type == XPATH_XSLT_TREE) ||
      (arg1->type == XPATH_NODESET) || (arg1->type == XPATH_XSLT_TREE)) {
	 
	if ((arg1->type != XPATH_NODESET) && (arg1->type != XPATH_XSLT_TREE)) {
		argtmp = arg2;
		arg2 = arg1;
		arg1 = argtmp;
	}
	switch (arg2->type) {
	    case XPATH_UNDEFINED:
#ifdef DEBUG_EXPR
		xmlGenericError(xmlGenericErrorContext,
			"Equal: undefined\n");
#endif
		break;
	    case XPATH_NODESET:
	    case XPATH_XSLT_TREE:
		ret = xmlXPathEqualNodeSets(arg1, arg2, 0);
		break;
	    case XPATH_BOOLEAN:
		if ((arg1->nodesetval == NULL) ||
		  (arg1->nodesetval->nodeNr == 0)) ret = 0;
		else
		    ret = 1;
		ret = (ret == arg2->boolval);
		break;
	    case XPATH_NUMBER:
		ret = xmlXPathEqualNodeSetFloat(ctxt, arg1, arg2->floatval, 0);
		break;
	    case XPATH_STRING:
		ret = xmlXPathEqualNodeSetString(arg1, arg2->stringval, 0);
		break;
	    case XPATH_USERS:
	    case XPATH_POINT:
	    case XPATH_RANGE:
	    case XPATH_LOCATIONSET:
		TODO
		break;
	}
	xmlXPathReleaseObject(ctxt->context, arg1);
	xmlXPathReleaseObject(ctxt->context, arg2);
	return(ret);
    }

    return (xmlXPathEqualValuesCommon(ctxt, arg1, arg2));
}
