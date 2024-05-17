xmlXPtrOriginFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);

    if (ctxt->context->origin == NULL)
	XP_ERROR(XPTR_SYNTAX_ERROR);
    
    valuePush(ctxt, xmlXPtrNewLocationSetNodes(ctxt->context->origin, NULL));
}
