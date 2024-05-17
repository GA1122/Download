xmlXPathReleaseObject(xmlXPathContextPtr ctxt, xmlXPathObjectPtr obj)
{
#define XP_CACHE_ADD(sl, o) if (sl == NULL) { \
	sl = xmlPointerListCreate(10); if (sl == NULL) goto free_obj; } \
    if (xmlPointerListAddSize(sl, obj, 0) == -1) goto free_obj;

#define XP_CACHE_WANTS(sl, n) ((sl == NULL) || ((sl)->number < n))

    if (obj == NULL)
	return;
    if ((ctxt == NULL) || (ctxt->cache == NULL)) {
	 xmlXPathFreeObject(obj);
    } else {
	xmlXPathContextCachePtr cache =
	    (xmlXPathContextCachePtr) ctxt->cache;

	switch (obj->type) {
	    case XPATH_NODESET:
	    case XPATH_XSLT_TREE:
		if (obj->nodesetval != NULL) {
		    if (obj->boolval) {
			 
			obj->type = XPATH_XSLT_TREE;  
			xmlXPathFreeValueTree(obj->nodesetval);
			obj->nodesetval = NULL;
		    } else if ((obj->nodesetval->nodeMax <= 40) &&
			(XP_CACHE_WANTS(cache->nodesetObjs,
					cache->maxNodeset)))
		    {
			XP_CACHE_ADD(cache->nodesetObjs, obj);
			goto obj_cached;
		    } else {
			xmlXPathFreeNodeSet(obj->nodesetval);
			obj->nodesetval = NULL;
		    }
		}
		break;
	    case XPATH_STRING:
		if (obj->stringval != NULL)
		    xmlFree(obj->stringval);

		if (XP_CACHE_WANTS(cache->stringObjs, cache->maxString)) {
		    XP_CACHE_ADD(cache->stringObjs, obj);
		    goto obj_cached;
		}
		break;
	    case XPATH_BOOLEAN:
		if (XP_CACHE_WANTS(cache->booleanObjs, cache->maxBoolean)) {
		    XP_CACHE_ADD(cache->booleanObjs, obj);
		    goto obj_cached;
		}
		break;
	    case XPATH_NUMBER:
		if (XP_CACHE_WANTS(cache->numberObjs, cache->maxNumber)) {
		    XP_CACHE_ADD(cache->numberObjs, obj);
		    goto obj_cached;
		}
		break;
#ifdef LIBXML_XPTR_ENABLED
	    case XPATH_LOCATIONSET:
		if (obj->user != NULL) {
		    xmlXPtrFreeLocationSet(obj->user);
		}
		goto free_obj;
#endif
	    default:
		goto free_obj;
	}

	 
	if (XP_CACHE_WANTS(cache->miscObjs, cache->maxMisc)) {
	    XP_CACHE_ADD(cache->miscObjs, obj);
	} else
	    goto free_obj;

obj_cached:

#ifdef XP_DEBUG_OBJ_USAGE
	xmlXPathDebugObjUsageReleased(ctxt, obj->type);
#endif

	if (obj->nodesetval != NULL) {
	    xmlNodeSetPtr tmpset = obj->nodesetval;

	     
	    if (tmpset->nodeNr > 1) {
		int i;
		xmlNodePtr node;

		for (i = 0; i < tmpset->nodeNr; i++) {
		    node = tmpset->nodeTab[i];
		    if ((node != NULL) &&
			(node->type == XML_NAMESPACE_DECL))
		    {
			xmlXPathNodeSetFreeNs((xmlNsPtr) node);
		    }
		}
	    } else if (tmpset->nodeNr == 1) {
		if ((tmpset->nodeTab[0] != NULL) &&
		    (tmpset->nodeTab[0]->type == XML_NAMESPACE_DECL))
		    xmlXPathNodeSetFreeNs((xmlNsPtr) tmpset->nodeTab[0]);
	    }
	    tmpset->nodeNr = 0;
	    memset(obj, 0, sizeof(xmlXPathObject));
	    obj->nodesetval = tmpset;
	} else
	    memset(obj, 0, sizeof(xmlXPathObject));

	return;

free_obj:
	 
	if (obj->nodesetval != NULL)
	    xmlXPathFreeNodeSet(obj->nodesetval);
#ifdef XP_DEBUG_OBJ_USAGE
	xmlXPathDebugObjUsageReleased(NULL, obj->type);
#endif
	xmlFree(obj);
    }
    return;
}
