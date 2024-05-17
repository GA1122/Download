xsltStyleStylesheetLevelGetExtData(xsltStylesheetPtr style,
				   const xmlChar * URI)
{
    xsltExtDataPtr dataContainer = NULL;

    if ((style == NULL) || (URI == NULL) ||
	(xsltExtensionsHash == NULL))
	return (NULL);

    if (style->extInfos != NULL) {
	dataContainer = (xsltExtDataPtr) xmlHashLookup(style->extInfos, URI);
	 
	if (dataContainer)
	    return(dataContainer->extData);
    }

    dataContainer =
        xsltStyleInitializeStylesheetModule(style, URI);
    if (dataContainer != NULL)
	return (dataContainer->extData);
    return(NULL);
}
