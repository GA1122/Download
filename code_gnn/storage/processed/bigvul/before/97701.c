xmlXPathLocalNameFunction(xmlXPathParserContextPtr ctxt, int nargs) {
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
	case XML_PI_NODE:
	    if (cur->nodesetval->nodeTab[i]->name[0] == ' ')
		valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
	    else
		valuePush(ctxt,
		      xmlXPathCacheNewString(ctxt->context,
			cur->nodesetval->nodeTab[i]->name));
	    break;
	case XML_NAMESPACE_DECL:
	    valuePush(ctxt, xmlXPathCacheNewString(ctxt->context,
			((xmlNsPtr)cur->nodesetval->nodeTab[i])->prefix));
	    break;
	default:
	    valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
	}
    }
    xmlXPathReleaseObject(ctxt->context, cur);
}
