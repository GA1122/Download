xmlXPathStringLengthFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur;

    if (nargs == 0) {
        if ((ctxt == NULL) || (ctxt->context == NULL))
	    return;
	if (ctxt->context->node == NULL) {
	    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context, 0));
	} else {
	    xmlChar *content;

	    content = xmlXPathCastNodeToString(ctxt->context->node);
	    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context,
		xmlUTF8Strlen(content)));
	    xmlFree(content);
	}
	return;
    }
    CHECK_ARITY(1);
    CAST_TO_STRING;
    CHECK_TYPE(XPATH_STRING);
    cur = valuePop(ctxt);
    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context,
	xmlUTF8Strlen(cur->stringval)));
    xmlXPathReleaseObject(ctxt->context, cur);
}
