xsltCreateRVT(xsltTransformContextPtr ctxt)
{
    xmlDocPtr container;

     
    if (ctxt == NULL)
	return(NULL);

     
    if (ctxt->cache->RVT) {
	container = ctxt->cache->RVT;
	ctxt->cache->RVT = (xmlDocPtr) container->next;
	 
	container->next = NULL;
	container->prev = NULL;
	if (ctxt->cache->nbRVT > 0)
	    ctxt->cache->nbRVT--;
#ifdef XSLT_DEBUG_PROFILE_CACHE
	ctxt->cache->dbgReusedRVTs++;
#endif
	return(container);
    }

    container = xmlNewDoc(NULL);
    if (container == NULL)
	return(NULL);
    container->dict = ctxt->dict;
    xmlDictReference(container->dict);
    XSLT_MARK_RES_TREE_FRAG(container);
    container->doc = container;
    container->parent = NULL;
    return(container);
}
