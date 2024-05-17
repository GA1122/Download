xmlXPathNewValueTree(xmlNodePtr val) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating result value tree\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_XSLT_TREE;
    ret->boolval = 1;
    ret->user = (void *) val;
    ret->nodesetval = xmlXPathNodeSetCreate(val);
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, XPATH_XSLT_TREE);
#endif
    return(ret);
}
