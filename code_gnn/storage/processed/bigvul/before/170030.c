xsltFreeStackElem(xsltStackElemPtr elem) {
    if (elem == NULL)
	return;
    if (elem->value != NULL)
	xmlXPathFreeObject(elem->value);
     
    if (elem->fragment) {
	xmlDocPtr cur;

	while (elem->fragment != NULL) {
	    cur = elem->fragment;
	    elem->fragment = (xmlDocPtr) cur->next;

	    if (elem->context &&
		(cur->psvi == (void *) ((long) 1)))
	    {
		 
		xsltRegisterLocalRVT(elem->context, cur);
	    } else {
		xsltReleaseRVT((xsltTransformContextPtr) elem->context,
		    cur);
	    }
	}
    }
     
    if (elem->context && (elem->context->cache->nbStackItems < 50)) {
	 
	xsltTransformContextPtr ctxt = elem->context;
	memset(elem, 0, sizeof(xsltStackElem));
	elem->context = ctxt;
	elem->next = ctxt->cache->stackItems;
	ctxt->cache->stackItems = elem;
	ctxt->cache->nbStackItems++;
#ifdef XSLT_DEBUG_PROFILE_CACHE
	ctxt->cache->dbgCachedVars++;
#endif
	return;
    }
    xmlFree(elem);
}
