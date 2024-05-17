xsltNewStackElem(xsltTransformContextPtr ctxt)
{
    xsltStackElemPtr ret;
     
    if (ctxt && ctxt->cache->stackItems) {
	ret = ctxt->cache->stackItems;
	ctxt->cache->stackItems = ret->next;
	ret->next = NULL;
	ctxt->cache->nbStackItems--;
#ifdef XSLT_DEBUG_PROFILE_CACHE
	ctxt->cache->dbgReusedVars++;
#endif
	return(ret);
    }
    ret = (xsltStackElemPtr) xmlMalloc(sizeof(xsltStackElem));
    if (ret == NULL) {
	xsltTransformError(NULL, NULL, NULL,
		"xsltNewStackElem : malloc failed\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(xsltStackElem));
    ret->context = ctxt;
    return(ret);
}
