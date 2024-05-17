xmlXPathEvalExpr(xmlXPathParserContextPtr ctxt) {
#ifdef XPATH_STREAMING
    xmlXPathCompExprPtr comp;
#endif

    if (ctxt == NULL) return;

#ifdef XPATH_STREAMING
    comp = xmlXPathTryStreamCompile(ctxt->context, ctxt->base);
    if (comp != NULL) {
        if (ctxt->comp != NULL)
	    xmlXPathFreeCompExpr(ctxt->comp);
        ctxt->comp = comp;
	if (ctxt->cur != NULL)
	    while (*ctxt->cur != 0) ctxt->cur++;
    } else
#endif
    {
	xmlXPathCompileExpr(ctxt, 1);
	 
	if ((ctxt->error == XPATH_EXPRESSION_OK) &&
	    (ctxt->comp != NULL) &&
	    (ctxt->base != NULL) &&
	    (ctxt->comp->nbStep > 2) &&
	    (ctxt->comp->last >= 0) &&
	    (xmlXPathCanRewriteDosExpression((xmlChar *) ctxt->base) == 1))
	{
	    xmlXPathRewriteDOSExpression(ctxt->comp,
		&ctxt->comp->steps[ctxt->comp->last]);
	}
    }
    CHECK_ERROR;
    xmlXPathRunEval(ctxt, 0);
}
