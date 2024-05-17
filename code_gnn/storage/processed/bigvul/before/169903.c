xsltNextImport(xsltStylesheetPtr cur) {
    if (cur == NULL)
	return(NULL);
    if (cur->imports != NULL)
	return(cur->imports);
    if (cur->next != NULL)
	return(cur->next) ;
    do {
	cur = cur->parent;
	if (cur == NULL) break;
	if (cur->next != NULL) return(cur->next);
    } while (cur != NULL);
    return(cur);
}
