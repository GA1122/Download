xmlXPtrGetArity(xmlNodePtr cur) {
    int i;
    if (cur == NULL) 
	return(-1);
    cur = cur->children;
    for (i = 0;cur != NULL;cur = cur->next) {
	if ((cur->type == XML_ELEMENT_NODE) ||
	    (cur->type == XML_DOCUMENT_NODE) ||
	    (cur->type == XML_HTML_DOCUMENT_NODE)) {
	    i++;
	}
    }
    return(i);
}
