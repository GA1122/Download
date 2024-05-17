xmlXPathEvalExpression(const xmlChar *str, xmlXPathContextPtr ctxt) {
    xmlXPathParserContextPtr pctxt;
    xmlXPathObjectPtr res, tmp;
    int stack = 0;

    CHECK_CTXT(ctxt)

    xmlXPathInit();

    pctxt = xmlXPathNewParserContext(str, ctxt);
    if (pctxt == NULL)
        return NULL;
    xmlXPathEvalExpr(pctxt);

    if ((*pctxt->cur != 0) || (pctxt->error != XPATH_EXPRESSION_OK)) {
	xmlXPatherror(pctxt, __FILE__, __LINE__, XPATH_EXPR_ERROR);
	res = NULL;
    } else {
	res = valuePop(pctxt);
    }
    do {
        tmp = valuePop(pctxt);
	if (tmp != NULL) {
	    xmlXPathReleaseObject(ctxt, tmp);
	    stack++;
	}
    } while (tmp != NULL);
    if ((stack != 0) && (res != NULL)) {
	xmlGenericError(xmlGenericErrorContext,
		"xmlXPathEvalExpression: %d object left on the stack\n",
	        stack);
    }
    xmlXPathFreeParserContext(pctxt);
    return(res);
}
