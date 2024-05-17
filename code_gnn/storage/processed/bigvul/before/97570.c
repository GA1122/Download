xmlXPathCacheNewBoolean(xmlXPathContextPtr ctxt, int val)
{
    if ((ctxt != NULL) && (ctxt->cache)) {
	xmlXPathContextCachePtr cache = (xmlXPathContextCachePtr) ctxt->cache;

	if ((cache->booleanObjs != NULL) &&
	    (cache->booleanObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->booleanObjs->items[--cache->booleanObjs->number];
	    ret->type = XPATH_BOOLEAN;
	    ret->boolval = (val != 0);
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_BOOLEAN);
#endif
	    return(ret);
	} else if ((cache->miscObjs != NULL) &&
	    (cache->miscObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->miscObjs->items[--cache->miscObjs->number];

	    ret->type = XPATH_BOOLEAN;
	    ret->boolval = (val != 0);
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_BOOLEAN);
#endif
	    return(ret);
	}
    }
    return(xmlXPathNewBoolean(val));
}
