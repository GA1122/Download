xmlXPathDebugObjUsageRequested(xmlXPathContextPtr ctxt,
			      xmlXPathObjectType objType)
{
    int isCached = 0;

    if (ctxt != NULL) {
	if (ctxt->cache != NULL) {
	    xmlXPathContextCachePtr cache =
		(xmlXPathContextCachePtr) ctxt->cache;

	    isCached = 1;

	    cache->dbgReusedAll++;
	    switch (objType) {
		case XPATH_UNDEFINED:
		    cache->dbgReusedUndefined++;
		    break;
		case XPATH_NODESET:
		    cache->dbgReusedNodeset++;
		    break;
		case XPATH_BOOLEAN:
		    cache->dbgReusedBool++;
		    break;
		case XPATH_NUMBER:
		    cache->dbgReusedNumber++;
		    break;
		case XPATH_STRING:
		    cache->dbgReusedString++;
		    break;
		case XPATH_POINT:
		    cache->dbgReusedPoint++;
		    break;
		case XPATH_RANGE:
		    cache->dbgReusedRange++;
		    break;
		case XPATH_LOCATIONSET:
		    cache->dbgReusedLocset++;
		    break;
		case XPATH_USERS:
		    cache->dbgReusedUsers++;
		    break;
		case XPATH_XSLT_TREE:
		    cache->dbgReusedXSLTTree++;
		    break;
		default:
		    break;
	    }
	}
    }

    switch (objType) {
	case XPATH_UNDEFINED:
	    if (! isCached)
		xmlXPathDebugObjTotalUndefined++;
	    xmlXPathDebugObjCounterUndefined++;
	    if (xmlXPathDebugObjCounterUndefined >
		xmlXPathDebugObjMaxUndefined)
		xmlXPathDebugObjMaxUndefined =
		    xmlXPathDebugObjCounterUndefined;
	    break;
	case XPATH_NODESET:
	    if (! isCached)
		xmlXPathDebugObjTotalNodeset++;
	    xmlXPathDebugObjCounterNodeset++;
	    if (xmlXPathDebugObjCounterNodeset >
		xmlXPathDebugObjMaxNodeset)
		xmlXPathDebugObjMaxNodeset =
		    xmlXPathDebugObjCounterNodeset;
	    break;
	case XPATH_BOOLEAN:
	    if (! isCached)
		xmlXPathDebugObjTotalBool++;
	    xmlXPathDebugObjCounterBool++;
	    if (xmlXPathDebugObjCounterBool >
		xmlXPathDebugObjMaxBool)
		xmlXPathDebugObjMaxBool =
		    xmlXPathDebugObjCounterBool;
	    break;
	case XPATH_NUMBER:
	    if (! isCached)
		xmlXPathDebugObjTotalNumber++;
	    xmlXPathDebugObjCounterNumber++;
	    if (xmlXPathDebugObjCounterNumber >
		xmlXPathDebugObjMaxNumber)
		xmlXPathDebugObjMaxNumber =
		    xmlXPathDebugObjCounterNumber;
	    break;
	case XPATH_STRING:
	    if (! isCached)
		xmlXPathDebugObjTotalString++;
	    xmlXPathDebugObjCounterString++;
	    if (xmlXPathDebugObjCounterString >
		xmlXPathDebugObjMaxString)
		xmlXPathDebugObjMaxString =
		    xmlXPathDebugObjCounterString;
	    break;
	case XPATH_POINT:
	    if (! isCached)
		xmlXPathDebugObjTotalPoint++;
	    xmlXPathDebugObjCounterPoint++;
	    if (xmlXPathDebugObjCounterPoint >
		xmlXPathDebugObjMaxPoint)
		xmlXPathDebugObjMaxPoint =
		    xmlXPathDebugObjCounterPoint;
	    break;
	case XPATH_RANGE:
	    if (! isCached)
		xmlXPathDebugObjTotalRange++;
	    xmlXPathDebugObjCounterRange++;
	    if (xmlXPathDebugObjCounterRange >
		xmlXPathDebugObjMaxRange)
		xmlXPathDebugObjMaxRange =
		    xmlXPathDebugObjCounterRange;
	    break;
	case XPATH_LOCATIONSET:
	    if (! isCached)
		xmlXPathDebugObjTotalLocset++;
	    xmlXPathDebugObjCounterLocset++;
	    if (xmlXPathDebugObjCounterLocset >
		xmlXPathDebugObjMaxLocset)
		xmlXPathDebugObjMaxLocset =
		    xmlXPathDebugObjCounterLocset;
	    break;
	case XPATH_USERS:
	    if (! isCached)
		xmlXPathDebugObjTotalUsers++;
	    xmlXPathDebugObjCounterUsers++;
	    if (xmlXPathDebugObjCounterUsers >
		xmlXPathDebugObjMaxUsers)
		xmlXPathDebugObjMaxUsers =
		    xmlXPathDebugObjCounterUsers;
	    break;
	case XPATH_XSLT_TREE:
	    if (! isCached)
		xmlXPathDebugObjTotalXSLTTree++;
	    xmlXPathDebugObjCounterXSLTTree++;
	    if (xmlXPathDebugObjCounterXSLTTree >
		xmlXPathDebugObjMaxXSLTTree)
		xmlXPathDebugObjMaxXSLTTree =
		    xmlXPathDebugObjCounterXSLTTree;
	    break;
	default:
	    break;
    }
    if (! isCached)
	xmlXPathDebugObjTotalAll++;
    xmlXPathDebugObjCounterAll++;
    if (xmlXPathDebugObjCounterAll >
	xmlXPathDebugObjMaxAll)
	xmlXPathDebugObjMaxAll =
	    xmlXPathDebugObjCounterAll;
}
