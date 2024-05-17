xsltShutdownExts(xsltStylesheetPtr style)
{
    if (style == NULL)
        return;
    if (style->extInfos == NULL)
        return;
    xmlHashScan(style->extInfos, (xmlHashScanner) xsltShutdownExt, style);
    xmlHashFree(style->extInfos, (xmlHashDeallocator) xsltFreeExtData);
    style->extInfos = NULL;
}
