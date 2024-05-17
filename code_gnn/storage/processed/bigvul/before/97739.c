xmlXPathNodeSetCreate(xmlNodePtr val) {
    xmlNodeSetPtr ret;

    ret = (xmlNodeSetPtr) xmlMalloc(sizeof(xmlNodeSet));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating nodeset\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlNodeSet));
    if (val != NULL) {
        ret->nodeTab = (xmlNodePtr *) xmlMalloc(XML_NODESET_DEFAULT *
					     sizeof(xmlNodePtr));
	if (ret->nodeTab == NULL) {
	    xmlXPathErrMemory(NULL, "creating nodeset\n");
	    xmlFree(ret);
	    return(NULL);
	}
	memset(ret->nodeTab, 0 ,
	       XML_NODESET_DEFAULT * (size_t) sizeof(xmlNodePtr));
        ret->nodeMax = XML_NODESET_DEFAULT;
	if (val->type == XML_NAMESPACE_DECL) {
	    xmlNsPtr ns = (xmlNsPtr) val;

	    ret->nodeTab[ret->nodeNr++] =
		xmlXPathNodeSetDupNs((xmlNodePtr) ns->next, ns);
	} else
	    ret->nodeTab[ret->nodeNr++] = val;
    }
    return(ret);
}
