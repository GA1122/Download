xsltAddAttrElemList(xsltAttrElemPtr list, xmlNodePtr attr) {
    xsltAttrElemPtr next, cur;

    if (attr == NULL)
	return(list);
    if (list == NULL)
	return(xsltNewAttrElem(attr));
    cur = list;
    while (cur != NULL) {
	next = cur->next;
	if (cur->attr == attr)
	    return(cur);
	if (cur->next == NULL) {
	    cur->next = xsltNewAttrElem(attr);
	    return(list);
	}
	cur = next;
    }
    return(list);
}
