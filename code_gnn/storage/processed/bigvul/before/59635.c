testXPath(const char *str, int xptr, int expr) {
    xmlGenericErrorFunc handler = ignoreGenericError;
    xmlXPathObjectPtr res;
    xmlXPathContextPtr ctxt;

     
    initGenericErrorDefaultFunc(&handler);

    nb_tests++;
#if defined(LIBXML_XPTR_ENABLED)
    if (xptr) {
	ctxt = xmlXPtrNewContext(xpathDocument, NULL, NULL);
	res = xmlXPtrEval(BAD_CAST str, ctxt);
    } else {
#endif
	ctxt = xmlXPathNewContext(xpathDocument);
	ctxt->node = xmlDocGetRootElement(xpathDocument);
	if (expr)
	    res = xmlXPathEvalExpression(BAD_CAST str, ctxt);
	else {
	     
	    xmlXPathCompExprPtr comp;

	    comp = xmlXPathCompile(BAD_CAST str);
	    if (comp != NULL) {
		res = xmlXPathCompiledEval(comp, ctxt);
		xmlXPathFreeCompExpr(comp);
	    } else
		res = NULL;
	}
#if defined(LIBXML_XPTR_ENABLED)
    }
#endif
    xmlXPathDebugDumpObject(xpathOutput, res, 0);
    xmlXPathFreeObject(res);
    xmlXPathFreeContext(ctxt);

     
    initGenericErrorDefaultFunc(NULL);
}