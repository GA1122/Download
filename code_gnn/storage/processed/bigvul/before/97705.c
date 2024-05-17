xmlXPathNamespaceURIFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur;

    if (ctxt == NULL) return;

    if (nargs == 0) {
	valuePush(ctxt, xmlXPathCacheNewNodeSet(ctxt->context,
	    ctxt->context->node));
	nargs = 1;
    }
    CHECK_ARITY(1);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_NODESET) &&
	 (ctxt->value->type != XPATH_XSLT_TREE)))
	XP_ERROR(XPATH_INVALID_TYPE);
    cur = valuePop(ctxt);

    if ((cur->nodesetval == NULL) || (cur->nodesetval->nodeNr == 0)) {
	valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
    } else {
	int i = 0;  
	switch (cur->nodesetval->nodeTab[i]->type) {
	case XML_ELEMENT_NODE:
	case XML_ATTRIBUTE_NODE:
	    if (cur->nodesetval->nodeTab[i]->ns == NULL)
		valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
	    else
		valuePush(ctxt, xmlXPathCacheNewString(ctxt->context,
			  cur->nodesetval->nodeTab[i]->ns->href));
	    break;
	default:
	    valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
	}
    }
    xmlXPathReleaseObject(ctxt->context, cur);
}
