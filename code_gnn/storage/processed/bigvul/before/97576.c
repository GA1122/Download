xmlXPathCacheWrapNodeSet(xmlXPathContextPtr ctxt, xmlNodeSetPtr val)
{
    if ((ctxt != NULL) && (ctxt->cache != NULL)) {
	xmlXPathContextCachePtr cache =
	    (xmlXPathContextCachePtr) ctxt->cache;

	if ((cache->miscObjs != NULL) &&
	    (cache->miscObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->miscObjs->items[--cache->miscObjs->number];
	    ret->type = XPATH_NODESET;
	    ret->nodesetval = val;
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_NODESET);
#endif
	    return(ret);
	}
    }

    return(xmlXPathWrapNodeSet(val));

}
