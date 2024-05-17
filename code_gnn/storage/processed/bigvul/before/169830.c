xsltFreeAttrElemList(xsltAttrElemPtr list) {
    xsltAttrElemPtr next;

    while (list != NULL) {
	next = list->next;
	xsltFreeAttrElem(list);
	list = next;
    }
}
