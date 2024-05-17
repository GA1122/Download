xmlXPtrAdvanceNode(xmlNodePtr cur, int *level) {
next:
    if (cur == NULL)
	return(NULL);
    if (cur->children != NULL) {
        cur = cur->children ;
	if (level != NULL)
	    (*level)++;
	goto found;
    }
skip:		 
    if (cur->next != NULL) {
	cur = cur->next;
	goto found;
    }
    do {
        cur = cur->parent;
	if (level != NULL)
	    (*level)--;
        if (cur == NULL) return(NULL);
        if (cur->next != NULL) {
	    cur = cur->next;
	    goto found;
	}
    } while (cur != NULL);

found:
    if ((cur->type != XML_ELEMENT_NODE) &&
	(cur->type != XML_TEXT_NODE) &&
	(cur->type != XML_DOCUMENT_NODE) &&
	(cur->type != XML_HTML_DOCUMENT_NODE) &&
	(cur->type != XML_CDATA_SECTION_NODE)) {
	    if (cur->type == XML_ENTITY_REF_NODE) {	 
		TODO
		goto skip;
	    }
	    goto next;
	}
    return(cur);
}
