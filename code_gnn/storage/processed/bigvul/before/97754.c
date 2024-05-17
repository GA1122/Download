xmlXPathNumberFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur;
    double res;

    if (ctxt == NULL) return;
    if (nargs == 0) {
	if (ctxt->context->node == NULL) {
	    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context, 0.0));
	} else {
	    xmlChar* content = xmlNodeGetContent(ctxt->context->node);

	    res = xmlXPathStringEvalNumber(content);
	    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context, res));
	    xmlFree(content);
	}
	return;
    }

    CHECK_ARITY(1);
    cur = valuePop(ctxt);
    valuePush(ctxt, xmlXPathCacheConvertNumber(ctxt->context, cur));
}
