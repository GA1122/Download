xmlXPathCacheWrapString(xmlXPathContextPtr ctxt, xmlChar *val)
{
    if ((ctxt != NULL) && (ctxt->cache != NULL)) {
	xmlXPathContextCachePtr cache = (xmlXPathContextCachePtr) ctxt->cache;

	if ((cache->stringObjs != NULL) &&
	    (cache->stringObjs->number != 0))
	{

	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->stringObjs->items[--cache->stringObjs->number];
	    ret->type = XPATH_STRING;
	    ret->stringval = val;
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_STRING);
#endif
	    return(ret);
	} else if ((cache->miscObjs != NULL) &&
	    (cache->miscObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;
	     
	    ret = (xmlXPathObjectPtr)
		cache->miscObjs->items[--cache->miscObjs->number];

	    ret->type = XPATH_STRING;
	    ret->stringval = val;
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_STRING);
#endif
	    return(ret);
	}
    }
    return(xmlXPathWrapString(val));
}
