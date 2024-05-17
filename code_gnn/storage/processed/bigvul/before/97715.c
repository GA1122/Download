xmlXPathNewString(const xmlChar *val) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating string object\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_STRING;
    if (val != NULL)
	ret->stringval = xmlStrdup(val);
    else
	ret->stringval = xmlStrdup((const xmlChar *)"");
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, XPATH_STRING);
#endif
    return(ret);
}
