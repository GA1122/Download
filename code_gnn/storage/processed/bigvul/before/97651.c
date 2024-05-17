xmlXPathDebugObjUsageDisplay(xmlXPathContextPtr ctxt)
{
    int reqAll, reqNodeset, reqString, reqBool, reqNumber,
	reqXSLTTree, reqUndefined;
    int caAll = 0, caNodeset = 0, caString = 0, caBool = 0,
	caNumber = 0, caXSLTTree = 0, caUndefined = 0;
    int reAll = 0, reNodeset = 0, reString = 0, reBool = 0,
	reNumber = 0, reXSLTTree = 0, reUndefined = 0;
    int leftObjs = xmlXPathDebugObjCounterAll;

    reqAll = xmlXPathDebugObjTotalAll;
    reqNodeset = xmlXPathDebugObjTotalNodeset;
    reqString = xmlXPathDebugObjTotalString;
    reqBool = xmlXPathDebugObjTotalBool;
    reqNumber = xmlXPathDebugObjTotalNumber;
    reqXSLTTree = xmlXPathDebugObjTotalXSLTTree;
    reqUndefined = xmlXPathDebugObjTotalUndefined;

    printf("# XPath object usage:\n");

    if (ctxt != NULL) {
	if (ctxt->cache != NULL) {
	    xmlXPathContextCachePtr cache =
		(xmlXPathContextCachePtr) ctxt->cache;

	    reAll = cache->dbgReusedAll;
	    reqAll += reAll;
	    reNodeset = cache->dbgReusedNodeset;
	    reqNodeset += reNodeset;
	    reString = cache->dbgReusedString;
	    reqString += reString;
	    reBool = cache->dbgReusedBool;
	    reqBool += reBool;
	    reNumber = cache->dbgReusedNumber;
	    reqNumber += reNumber;
	    reXSLTTree = cache->dbgReusedXSLTTree;
	    reqXSLTTree += reXSLTTree;
	    reUndefined = cache->dbgReusedUndefined;
	    reqUndefined += reUndefined;

	    caAll = cache->dbgCachedAll;
	    caBool = cache->dbgCachedBool;
	    caNodeset = cache->dbgCachedNodeset;
	    caString = cache->dbgCachedString;
	    caNumber = cache->dbgCachedNumber;
	    caXSLTTree = cache->dbgCachedXSLTTree;
	    caUndefined = cache->dbgCachedUndefined;

	    if (cache->nodesetObjs)
		leftObjs -= cache->nodesetObjs->number;
	    if (cache->stringObjs)
		leftObjs -= cache->stringObjs->number;
	    if (cache->booleanObjs)
		leftObjs -= cache->booleanObjs->number;
	    if (cache->numberObjs)
		leftObjs -= cache->numberObjs->number;
	    if (cache->miscObjs)
		leftObjs -= cache->miscObjs->number;
	}
    }

    printf("# all\n");
    printf("#   total  : %d\n", reqAll);
    printf("#   left  : %d\n", leftObjs);
    printf("#   created: %d\n", xmlXPathDebugObjTotalAll);
    printf("#   reused : %d\n", reAll);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxAll);

    printf("# node-sets\n");
    printf("#   total  : %d\n", reqNodeset);
    printf("#   created: %d\n", xmlXPathDebugObjTotalNodeset);
    printf("#   reused : %d\n", reNodeset);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxNodeset);

    printf("# strings\n");
    printf("#   total  : %d\n", reqString);
    printf("#   created: %d\n", xmlXPathDebugObjTotalString);
    printf("#   reused : %d\n", reString);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxString);

    printf("# booleans\n");
    printf("#   total  : %d\n", reqBool);
    printf("#   created: %d\n", xmlXPathDebugObjTotalBool);
    printf("#   reused : %d\n", reBool);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxBool);

    printf("# numbers\n");
    printf("#   total  : %d\n", reqNumber);
    printf("#   created: %d\n", xmlXPathDebugObjTotalNumber);
    printf("#   reused : %d\n", reNumber);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxNumber);

    printf("# XSLT result tree fragments\n");
    printf("#   total  : %d\n", reqXSLTTree);
    printf("#   created: %d\n", xmlXPathDebugObjTotalXSLTTree);
    printf("#   reused : %d\n", reXSLTTree);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxXSLTTree);

    printf("# undefined\n");
    printf("#   total  : %d\n", reqUndefined);
    printf("#   created: %d\n", xmlXPathDebugObjTotalUndefined);
    printf("#   reused : %d\n", reUndefined);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxUndefined);

}
