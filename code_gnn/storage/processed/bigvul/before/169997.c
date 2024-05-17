xsltFreeTransformContext(xsltTransformContextPtr ctxt) {
    if (ctxt == NULL)
	return;

     
    xsltShutdownCtxtExts(ctxt);

    if (ctxt->xpathCtxt != NULL) {
	ctxt->xpathCtxt->nsHash = NULL;
	xmlXPathFreeContext(ctxt->xpathCtxt);
    }
    if (ctxt->templTab != NULL)
	xmlFree(ctxt->templTab);
    if (ctxt->varsTab != NULL)
	xmlFree(ctxt->varsTab);
    if (ctxt->profTab != NULL)
	xmlFree(ctxt->profTab);
    if ((ctxt->extrasNr > 0) && (ctxt->extras != NULL)) {
	int i;

	for (i = 0;i < ctxt->extrasNr;i++) {
	    if ((ctxt->extras[i].deallocate != NULL) &&
		(ctxt->extras[i].info != NULL))
		ctxt->extras[i].deallocate(ctxt->extras[i].info);
	}
	xmlFree(ctxt->extras);
    }
    xsltFreeGlobalVariables(ctxt);
    xsltFreeDocuments(ctxt);
    xsltFreeCtxtExts(ctxt);
    xsltFreeRVTs(ctxt);
    xsltTransformCacheFree(ctxt->cache);
    xmlDictFree(ctxt->dict);
#ifdef WITH_XSLT_DEBUG
    xsltGenericDebug(xsltGenericDebugContext,
                     "freeing transformation dictionary\n");
#endif
    memset(ctxt, -1, sizeof(xsltTransformContext));
    xmlFree(ctxt);
}
