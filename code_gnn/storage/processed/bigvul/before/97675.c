xmlXPathFreeCache(xmlXPathContextCachePtr cache)
{
    if (cache == NULL)
	return;
    if (cache->nodesetObjs)
	xmlXPathCacheFreeObjectList(cache->nodesetObjs);
    if (cache->stringObjs)
	xmlXPathCacheFreeObjectList(cache->stringObjs);
    if (cache->booleanObjs)
	xmlXPathCacheFreeObjectList(cache->booleanObjs);
    if (cache->numberObjs)
	xmlXPathCacheFreeObjectList(cache->numberObjs);
    if (cache->miscObjs)
	xmlXPathCacheFreeObjectList(cache->miscObjs);
    xmlFree(cache);
}
