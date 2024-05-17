xmlXPtrGetLastChar(xmlNodePtr *node, int *indx) {
    xmlNodePtr cur;
    int pos, len = 0;

    if ((node == NULL) || (indx == NULL))
	return(-1);
    cur = *node;
    pos = *indx;

    if (cur == NULL)
	return(-1);

    if ((cur->type == XML_ELEMENT_NODE) ||
	(cur->type == XML_DOCUMENT_NODE) ||
	(cur->type == XML_HTML_DOCUMENT_NODE)) {
	if (pos > 0) {
	    cur = xmlXPtrGetNthChild(cur, pos);
	}
    }
    while (cur != NULL) {
	if (cur->last != NULL)
	    cur = cur->last;
	else if ((cur->type != XML_ELEMENT_NODE) &&
	         (cur->content != NULL)) {
	    len = xmlStrlen(cur->content);
	    break;
	} else {
	    return(-1);
	}
    }
    if (cur == NULL)
	return(-1);
    *node = cur;
    *indx = len;
    return(0);
}
