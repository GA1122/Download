xsltCopyStackElem(xsltStackElemPtr elem) {
    xsltStackElemPtr cur;

    cur = (xsltStackElemPtr) xmlMalloc(sizeof(xsltStackElem));
    if (cur == NULL) {
	xsltTransformError(NULL, NULL, NULL,
		"xsltCopyStackElem : malloc failed\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xsltStackElem));
    cur->context = elem->context;
    cur->name = elem->name;
    cur->nameURI = elem->nameURI;
    cur->select = elem->select;
    cur->tree = elem->tree;
    cur->comp = elem->comp;
    return(cur);
}
