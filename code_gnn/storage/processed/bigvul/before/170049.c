exclPrefixPush(xsltStylesheetPtr style, xmlChar * value)
{
    int i;

    if (style->exclPrefixMax == 0) {
        style->exclPrefixMax = 4;
        style->exclPrefixTab =
            (xmlChar * *)xmlMalloc(style->exclPrefixMax *
                                   sizeof(style->exclPrefixTab[0]));
        if (style->exclPrefixTab == NULL) {
            xmlGenericError(xmlGenericErrorContext, "malloc failed !\n");
            return (-1);
        }
    }
     
    for (i = 0;i < style->exclPrefixNr;i++) {
        if (xmlStrEqual(style->exclPrefixTab[i], value))
	    return(-1);
    }
    if (style->exclPrefixNr >= style->exclPrefixMax) {
        style->exclPrefixMax *= 2;
        style->exclPrefixTab =
            (xmlChar * *)xmlRealloc(style->exclPrefixTab,
                                    style->exclPrefixMax *
                                    sizeof(style->exclPrefixTab[0]));
        if (style->exclPrefixTab == NULL) {
            xmlGenericError(xmlGenericErrorContext, "realloc failed !\n");
            return (-1);
        }
    }
    style->exclPrefixTab[style->exclPrefixNr] = value;
    style->exclPrefix = value;
    return (style->exclPrefixNr++);
}
