xmlXPathLastFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
    if (ctxt->context->contextSize >= 0) {
	valuePush(ctxt,
	    xmlXPathCacheNewFloat(ctxt->context,
		(double) ctxt->context->contextSize));
#ifdef DEBUG_EXPR
	xmlGenericError(xmlGenericErrorContext,
		"last() : %d\n", ctxt->context->contextSize);
#endif
    } else {
	XP_ERROR(XPATH_INVALID_CTXT_SIZE);
    }
}
