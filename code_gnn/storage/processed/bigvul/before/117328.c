xmlXPtrNewPoint(xmlNodePtr node, int indx) {
    xmlXPathObjectPtr ret;

    if (node == NULL)
	return(NULL);
    if (indx < 0)
	return(NULL);

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating point");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_POINT;
    ret->user = (void *) node;
    ret->index = indx;
    return(ret);
}
