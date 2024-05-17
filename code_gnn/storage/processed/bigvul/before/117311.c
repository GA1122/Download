xmlXPtrGetIndex(xmlNodePtr cur) {
    int i;
    if (cur == NULL) 
	return(-1);
    for (i = 1;cur != NULL;cur = cur->prev) {
	if ((cur->type == XML_ELEMENT_NODE) ||
	    (cur->type == XML_DOCUMENT_NODE) ||
	    (cur->type == XML_HTML_DOCUMENT_NODE)) {
	    i++;
	}
    }
    return(i);
}
