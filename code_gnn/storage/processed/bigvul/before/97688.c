xmlXPathIdFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlChar *tokens;
    xmlNodeSetPtr ret;
    xmlXPathObjectPtr obj;

    CHECK_ARITY(1);
    obj = valuePop(ctxt);
    if (obj == NULL) XP_ERROR(XPATH_INVALID_OPERAND);
    if ((obj->type == XPATH_NODESET) || (obj->type == XPATH_XSLT_TREE)) {
	xmlNodeSetPtr ns;
	int i;

	ret = xmlXPathNodeSetCreate(NULL);
         

	if (obj->nodesetval != NULL) {
	    for (i = 0; i < obj->nodesetval->nodeNr; i++) {
		tokens =
		    xmlXPathCastNodeToString(obj->nodesetval->nodeTab[i]);
		ns = xmlXPathGetElementsByIds(ctxt->context->doc, tokens);
		ret = xmlXPathNodeSetMerge(ret, ns);
		xmlXPathFreeNodeSet(ns);
		if (tokens != NULL)
		    xmlFree(tokens);
	    }
	}
	xmlXPathReleaseObject(ctxt->context, obj);
	valuePush(ctxt, xmlXPathCacheWrapNodeSet(ctxt->context, ret));
	return;
    }
    obj = xmlXPathCacheConvertString(ctxt->context, obj);
    ret = xmlXPathGetElementsByIds(ctxt->context->doc, obj->stringval);
    valuePush(ctxt, xmlXPathCacheWrapNodeSet(ctxt->context, ret));
    xmlXPathReleaseObject(ctxt->context, obj);
    return;
}
