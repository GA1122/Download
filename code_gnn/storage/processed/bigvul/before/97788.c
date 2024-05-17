xmlXPathStringFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur;

    if (ctxt == NULL) return;
    if (nargs == 0) {
    valuePush(ctxt,
	xmlXPathCacheWrapString(ctxt->context,
	    xmlXPathCastNodeToString(ctxt->context->node)));
	return;
    }

    CHECK_ARITY(1);
    cur = valuePop(ctxt);
    if (cur == NULL) XP_ERROR(XPATH_INVALID_OPERAND);
    valuePush(ctxt, xmlXPathCacheConvertString(ctxt->context, cur));
}
