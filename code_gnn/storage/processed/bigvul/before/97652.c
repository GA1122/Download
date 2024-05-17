xmlXPathDebugObjUsageReleased(xmlXPathContextPtr ctxt,
			      xmlXPathObjectType objType)
{
    int isCached = 0;

    if (ctxt != NULL) {
	if (ctxt->cache != NULL) {
	    xmlXPathContextCachePtr cache =
		(xmlXPathContextCachePtr) ctxt->cache;

	    isCached = 1;

	    cache->dbgCachedAll++;
	    switch (objType) {
		case XPATH_UNDEFINED:
		    cache->dbgCachedUndefined++;
		    break;
		case XPATH_NODESET:
		    cache->dbgCachedNodeset++;
		    break;
		case XPATH_BOOLEAN:
		    cache->dbgCachedBool++;
		    break;
		case XPATH_NUMBER:
		    cache->dbgCachedNumber++;
		    break;
		case XPATH_STRING:
		    cache->dbgCachedString++;
		    break;
		case XPATH_POINT:
		    cache->dbgCachedPoint++;
		    break;
		case XPATH_RANGE:
		    cache->dbgCachedRange++;
		    break;
		case XPATH_LOCATIONSET:
		    cache->dbgCachedLocset++;
		    break;
		case XPATH_USERS:
		    cache->dbgCachedUsers++;
		    break;
		case XPATH_XSLT_TREE:
		    cache->dbgCachedXSLTTree++;
		    break;
		default:
		    break;
	    }

	}
    }
    switch (objType) {
	case XPATH_UNDEFINED:
	    xmlXPathDebugObjCounterUndefined--;
	    break;
	case XPATH_NODESET:
	    xmlXPathDebugObjCounterNodeset--;
	    break;
	case XPATH_BOOLEAN:
	    xmlXPathDebugObjCounterBool--;
	    break;
	case XPATH_NUMBER:
	    xmlXPathDebugObjCounterNumber--;
	    break;
	case XPATH_STRING:
	    xmlXPathDebugObjCounterString--;
	    break;
	case XPATH_POINT:
	    xmlXPathDebugObjCounterPoint--;
	    break;
	case XPATH_RANGE:
	    xmlXPathDebugObjCounterRange--;
	    break;
	case XPATH_LOCATIONSET:
	    xmlXPathDebugObjCounterLocset--;
	    break;
	case XPATH_USERS:
	    xmlXPathDebugObjCounterUsers--;
	    break;
	case XPATH_XSLT_TREE:
	    xmlXPathDebugObjCounterXSLTTree--;
	    break;
	default:
	    break;
    }
    xmlXPathDebugObjCounterAll--;
}
