xsltCheckExtPrefix(xsltStylesheetPtr style, const xmlChar * URI)
{
#ifdef XSLT_REFACTORED
    if ((style == NULL) || (style->compCtxt == NULL) ||
	(XSLT_CCTXT(style)->inode == NULL) ||
	(XSLT_CCTXT(style)->inode->extElemNs == NULL))
        return (0);
     
    if (XSLT_CCTXT(style)->inode->extElemNs != NULL) {
	int i;
	xsltPointerListPtr list = XSLT_CCTXT(style)->inode->extElemNs;

	for (i = 0; i < list->number; i++) {
	    if (xmlStrEqual((const xmlChar *) list->items[i],
		URI))
	    {
		return(1);
	    }
	}
    }
#else
    xsltExtDefPtr cur;

    if ((style == NULL) || (style->nsDefs == NULL))
        return (0);
    if (URI == NULL)
        URI = BAD_CAST "#default";
    cur = (xsltExtDefPtr) style->nsDefs;
    while (cur != NULL) {
	 
        if (xmlStrEqual(URI, cur->prefix))
            return (1);
        cur = cur->next;
    }
#endif
    return (0);
}
