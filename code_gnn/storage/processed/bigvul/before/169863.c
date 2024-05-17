xsltFreeExts(xsltStylesheetPtr style)
{
    if (style->nsDefs != NULL)
        xsltFreeExtDefList((xsltExtDefPtr) style->nsDefs);
}
