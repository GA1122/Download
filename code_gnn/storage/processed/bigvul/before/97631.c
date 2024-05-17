xmlXPathCompiledEvalInternal(xmlXPathCompExprPtr comp,
			     xmlXPathContextPtr ctxt,
			     xmlXPathObjectPtr *resObj,
			     int toBool)
{
    xmlXPathParserContextPtr pctxt;
#ifndef LIBXML_THREAD_ENABLED
    static int reentance = 0;
#endif
    int res;

    CHECK_CTXT_NEG(ctxt)

    if (comp == NULL)
	return(-1);
    xmlXPathInit();

#ifndef LIBXML_THREAD_ENABLED
    reentance++;
    if (reentance > 1)
	xmlXPathDisableOptimizer = 1;
#endif

#ifdef DEBUG_EVAL_COUNTS
    comp->nb++;
    if ((comp->string != NULL) && (comp->nb > 100)) {
	fprintf(stderr, "100 x %s\n", comp->string);
	comp->nb = 0;
    }
#endif
    pctxt = xmlXPathCompParserContext(comp, ctxt);
    res = xmlXPathRunEval(pctxt, toBool);

    if (resObj) {
	if (pctxt->value == NULL) {
	    xmlGenericError(xmlGenericErrorContext,
		"xmlXPathCompiledEval: evaluation failed\n");
	    *resObj = NULL;
	} else {
	    *resObj = valuePop(pctxt);
	}
    }

     
    if (pctxt->valueNr > 0) {
	xmlXPathObjectPtr tmp;
	int stack = 0;

	do {
	    tmp = valuePop(pctxt);
	    if (tmp != NULL) {
		stack++;
		xmlXPathReleaseObject(ctxt, tmp);
	    }
	} while (tmp != NULL);
	if ((stack != 0) &&
	    ((toBool) || ((resObj) && (*resObj))))
	{
	    xmlGenericError(xmlGenericErrorContext,
		"xmlXPathCompiledEval: %d objects left on the stack.\n",
		stack);
	}
    }

    if ((pctxt->error != XPATH_EXPRESSION_OK) && (resObj) && (*resObj)) {
	xmlXPathFreeObject(*resObj);
	*resObj = NULL;
    }
    pctxt->comp = NULL;
    xmlXPathFreeParserContext(pctxt);
#ifndef LIBXML_THREAD_ENABLED
    reentance--;
#endif

    return(res);
}
