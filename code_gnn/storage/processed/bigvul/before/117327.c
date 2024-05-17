xmlXPtrNewLocationSetNodes(xmlNodePtr start, xmlNodePtr end) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating locationset");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_LOCATIONSET;
    if (end == NULL)
	ret->user = xmlXPtrLocationSetCreate(xmlXPtrNewCollapsedRange(start));
    else
	ret->user = xmlXPtrLocationSetCreate(xmlXPtrNewRangeNodes(start,end));
    return(ret);
}
