xmlXPtrAdvanceChar(xmlNodePtr *node, int *indx, int bytes) {
    xmlNodePtr cur;
    int pos;
    int len;

    if ((node == NULL) || (indx == NULL))
	return(-1);
    cur = *node;
    if (cur == NULL)
	return(-1);
    pos = *indx;

    while (bytes >= 0) {
	 
	while ((cur != NULL) &&
	       ((cur->type == XML_ELEMENT_NODE) ||
	        (cur->type == XML_DOCUMENT_NODE) ||
	        (cur->type == XML_HTML_DOCUMENT_NODE))) {
	    if (pos > 0) {
		cur = xmlXPtrGetNthChild(cur, pos);
		pos = 0;
	    } else {
		cur = xmlXPtrAdvanceNode(cur, NULL);
		pos = 0;
	    }
	}

	if (cur == NULL) {
	    *node = NULL;
	    *indx = 0;
	    return(-1);
	}

	 
	if (pos == 0) pos = 1;
	if (bytes == 0) {
	    *node = cur;
	    *indx = pos;
	    return(0);
	}
	 
	len = 0;
	if ((cur->type != XML_ELEMENT_NODE) &&
            (cur->content != NULL)) {
	    len = xmlStrlen(cur->content);
	}
	if (pos > len) {
	     
	    STRANGE
	    pos = len;
	}
	if (pos + bytes >= len) {
	    bytes -= (len - pos);
	    cur = xmlXPtrAdvanceNode(cur, NULL);
	    pos = 0;
	} else if (pos + bytes < len) {
	    pos += bytes;
	    *node = cur;
	    *indx = pos;
	    return(0);
	}
    }
    return(-1);
}
