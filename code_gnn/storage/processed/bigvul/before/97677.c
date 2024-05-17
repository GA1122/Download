xmlXPathFreeContext(xmlXPathContextPtr ctxt) {
    if (ctxt == NULL) return;

    if (ctxt->cache != NULL)
	xmlXPathFreeCache((xmlXPathContextCachePtr) ctxt->cache);
    xmlXPathRegisteredNsCleanup(ctxt);
    xmlXPathRegisteredFuncsCleanup(ctxt);
    xmlXPathRegisteredVariablesCleanup(ctxt);
    xmlResetError(&ctxt->lastError);
    xmlFree(ctxt);
}
