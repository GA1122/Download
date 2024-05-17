xsltCheckExtURI(xsltStylesheetPtr style, const xmlChar * URI)
{
    xsltExtDefPtr cur;

    if ((style == NULL) || (style->nsDefs == NULL))
        return (0);
    if (URI == NULL)
        return (0);
    cur = (xsltExtDefPtr) style->nsDefs;
    while (cur != NULL) {
        if (xmlStrEqual(URI, cur->URI))
            return (1);
        cur = cur->next;
    }
    return (0);
}
