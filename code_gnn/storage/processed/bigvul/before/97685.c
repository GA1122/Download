xmlXPathGetElementsByIds (xmlDocPtr doc, const xmlChar *ids) {
    xmlNodeSetPtr ret;
    const xmlChar *cur = ids;
    xmlChar *ID;
    xmlAttrPtr attr;
    xmlNodePtr elem = NULL;

    if (ids == NULL) return(NULL);

    ret = xmlXPathNodeSetCreate(NULL);
    if (ret == NULL)
        return(ret);

    while (IS_BLANK_CH(*cur)) cur++;
    while (*cur != 0) {
	while ((!IS_BLANK_CH(*cur)) && (*cur != 0))
	    cur++;

        ID = xmlStrndup(ids, cur - ids);
	if (ID != NULL) {
	     
	    attr = xmlGetID(doc, ID);
	    if (attr != NULL) {
		if (attr->type == XML_ATTRIBUTE_NODE)
		    elem = attr->parent;
		else if (attr->type == XML_ELEMENT_NODE)
		    elem = (xmlNodePtr) attr;
		else
		    elem = NULL;
		if (elem != NULL)
		    xmlXPathNodeSetAdd(ret, elem);
	    }
	    xmlFree(ID);
	}

	while (IS_BLANK_CH(*cur)) cur++;
	ids = cur;
    }
    return(ret);
}
