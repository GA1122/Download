xsltReleaseRVT(xsltTransformContextPtr ctxt, xmlDocPtr RVT)
{
    if (RVT == NULL)
	return;

    if (ctxt && (ctxt->cache->nbRVT < 40)) {
	 
	if (RVT->_private != NULL) {
	    xsltFreeDocumentKeys((xsltDocumentPtr) RVT->_private);
	    xmlFree(RVT->_private);
	    RVT->_private = NULL;
	}
	 
	if (RVT->children != NULL) {
	    xmlFreeNodeList(RVT->children);
	    RVT->children = NULL;
	    RVT->last = NULL;
	}
	if (RVT->ids != NULL) {
	    xmlFreeIDTable((xmlIDTablePtr) RVT->ids);
	    RVT->ids = NULL;
	}
	if (RVT->refs != NULL) {
	    xmlFreeRefTable((xmlRefTablePtr) RVT->refs);
	    RVT->refs = NULL;
	}

	 
	RVT->psvi = 0;

	RVT->next = (xmlNodePtr) ctxt->cache->RVT;
	ctxt->cache->RVT = RVT;

	ctxt->cache->nbRVT++;

#ifdef XSLT_DEBUG_PROFILE_CACHE
	ctxt->cache->dbgCachedRVTs++;
#endif
	return;
    }
     
    if (RVT->_private != NULL) {
	xsltFreeDocumentKeys((xsltDocumentPtr) RVT->_private);
	xmlFree(RVT->_private);
    }
    xmlFreeDoc(RVT);
}
