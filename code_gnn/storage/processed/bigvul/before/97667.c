xmlXPathEval(const xmlChar *str, xmlXPathContextPtr ctx) {
    xmlXPathParserContextPtr ctxt;
    xmlXPathObjectPtr res, tmp, init = NULL;
    int stack = 0;

    CHECK_CTXT(ctx)

    xmlXPathInit();

    ctxt = xmlXPathNewParserContext(str, ctx);
    if (ctxt == NULL)
        return NULL;
    xmlXPathEvalExpr(ctxt);

    if (ctxt->value == NULL) {
	xmlGenericError(xmlGenericErrorContext,
		"xmlXPathEval: evaluation failed\n");
	res = NULL;
    } else if ((*ctxt->cur != 0) && (ctxt->comp != NULL)
#ifdef XPATH_STREAMING
            && (ctxt->comp->stream == NULL)
#endif
	      ) {
	xmlXPatherror(ctxt, __FILE__, __LINE__, XPATH_EXPR_ERROR);
	res = NULL;
    } else {
	res = valuePop(ctxt);
    }

    do {
        tmp = valuePop(ctxt);
	if (tmp != NULL) {
	    if (tmp != init)
		stack++;
	    xmlXPathReleaseObject(ctx, tmp);
        }
    } while (tmp != NULL);
    if ((stack != 0) && (res != NULL)) {
	xmlGenericError(xmlGenericErrorContext,
		"xmlXPathEval: %d object left on the stack\n",
	        stack);
    }
    if (ctxt->error != XPATH_EXPRESSION_OK) {
	xmlXPathFreeObject(res);
	res = NULL;
    }

    xmlXPathFreeParserContext(ctxt);
    return(res);
}
