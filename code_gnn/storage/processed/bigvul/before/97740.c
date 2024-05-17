xmlXPathNodeSetCreateSize(int size) {
    xmlNodeSetPtr ret;

    ret = (xmlNodeSetPtr) xmlMalloc(sizeof(xmlNodeSet));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating nodeset\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlNodeSet));
    if (size < XML_NODESET_DEFAULT)
	size = XML_NODESET_DEFAULT;
    ret->nodeTab = (xmlNodePtr *) xmlMalloc(size * sizeof(xmlNodePtr));
    if (ret->nodeTab == NULL) {
	xmlXPathErrMemory(NULL, "creating nodeset\n");
	xmlFree(ret);
	return(NULL);
    }
    memset(ret->nodeTab, 0 , size * (size_t) sizeof(xmlNodePtr));
    ret->nodeMax = size;
    return(ret);
}
