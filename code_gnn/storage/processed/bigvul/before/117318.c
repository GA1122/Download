xmlXPtrLocationSetCreate(xmlXPathObjectPtr val) {
    xmlLocationSetPtr ret;

    ret = (xmlLocationSetPtr) xmlMalloc(sizeof(xmlLocationSet));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating locationset");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlLocationSet));
    if (val != NULL) {
        ret->locTab = (xmlXPathObjectPtr *) xmlMalloc(XML_RANGESET_DEFAULT *
					     sizeof(xmlXPathObjectPtr));
	if (ret->locTab == NULL) {
	    xmlXPtrErrMemory("allocating locationset");
	    xmlFree(ret);
	    return(NULL);
	}
	memset(ret->locTab, 0 ,
	       XML_RANGESET_DEFAULT * (size_t) sizeof(xmlXPathObjectPtr));
        ret->locMax = XML_RANGESET_DEFAULT;
	ret->locTab[ret->locNr++] = val;
    }
    return(ret);
}
