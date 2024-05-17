xmlXPtrNbLocChildren(xmlNodePtr node) {
    int ret = 0;
    if (node == NULL)
	return(-1);
    switch (node->type) {
        case XML_HTML_DOCUMENT_NODE:
        case XML_DOCUMENT_NODE:
        case XML_ELEMENT_NODE:
	    node = node->children;
	    while (node != NULL) {
		if (node->type == XML_ELEMENT_NODE)
		    ret++;
		node = node->next;
	    }
	    break;
        case XML_ATTRIBUTE_NODE:
	    return(-1);

        case XML_PI_NODE:
        case XML_COMMENT_NODE:
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_ENTITY_REF_NODE:
	    ret = xmlStrlen(node->content);
	    break;
	default:
	    return(-1);
    }
    return(ret);
}
