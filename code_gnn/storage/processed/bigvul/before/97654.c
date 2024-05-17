xmlXPathDebugObjUsageReset(xmlXPathContextPtr ctxt)
{
    if (ctxt != NULL) {
	if (ctxt->cache != NULL) {
	    xmlXPathContextCachePtr cache =
		(xmlXPathContextCachePtr) ctxt->cache;

	    cache->dbgCachedAll = 0;
	    cache->dbgCachedNodeset = 0;
	    cache->dbgCachedString = 0;
	    cache->dbgCachedBool = 0;
	    cache->dbgCachedNumber = 0;
	    cache->dbgCachedPoint = 0;
	    cache->dbgCachedRange = 0;
	    cache->dbgCachedLocset = 0;
	    cache->dbgCachedUsers = 0;
	    cache->dbgCachedXSLTTree = 0;
	    cache->dbgCachedUndefined = 0;

	    cache->dbgReusedAll = 0;
	    cache->dbgReusedNodeset = 0;
	    cache->dbgReusedString = 0;
	    cache->dbgReusedBool = 0;
	    cache->dbgReusedNumber = 0;
	    cache->dbgReusedPoint = 0;
	    cache->dbgReusedRange = 0;
	    cache->dbgReusedLocset = 0;
	    cache->dbgReusedUsers = 0;
	    cache->dbgReusedXSLTTree = 0;
	    cache->dbgReusedUndefined = 0;
	}
    }

    xmlXPathDebugObjCounterUndefined = 0;
    xmlXPathDebugObjCounterNodeset = 0;
    xmlXPathDebugObjCounterBool = 0;
    xmlXPathDebugObjCounterNumber = 0;
    xmlXPathDebugObjCounterString = 0;
    xmlXPathDebugObjCounterPoint = 0;
    xmlXPathDebugObjCounterRange = 0;
    xmlXPathDebugObjCounterLocset = 0;
    xmlXPathDebugObjCounterUsers = 0;
    xmlXPathDebugObjCounterXSLTTree = 0;
    xmlXPathDebugObjCounterAll = 0;

    xmlXPathDebugObjTotalUndefined = 0;
    xmlXPathDebugObjTotalNodeset = 0;
    xmlXPathDebugObjTotalBool = 0;
    xmlXPathDebugObjTotalNumber = 0;
    xmlXPathDebugObjTotalString = 0;
    xmlXPathDebugObjTotalPoint = 0;
    xmlXPathDebugObjTotalRange = 0;
    xmlXPathDebugObjTotalLocset = 0;
    xmlXPathDebugObjTotalUsers = 0;
    xmlXPathDebugObjTotalXSLTTree = 0;
    xmlXPathDebugObjTotalAll = 0;

    xmlXPathDebugObjMaxUndefined = 0;
    xmlXPathDebugObjMaxNodeset = 0;
    xmlXPathDebugObjMaxBool = 0;
    xmlXPathDebugObjMaxNumber = 0;
    xmlXPathDebugObjMaxString = 0;
    xmlXPathDebugObjMaxPoint = 0;
    xmlXPathDebugObjMaxRange = 0;
    xmlXPathDebugObjMaxLocset = 0;
    xmlXPathDebugObjMaxUsers = 0;
    xmlXPathDebugObjMaxXSLTTree = 0;
    xmlXPathDebugObjMaxAll = 0;

}
