xsltNewNamespaceMapItem(xsltCompilerCtxtPtr cctxt,
			xmlDocPtr doc,
			xmlNsPtr ns,
			xmlNodePtr elem)
{
    xsltNsMapPtr ret;

    if ((cctxt == NULL) || (doc == NULL) || (ns == NULL))
	return(NULL);

    ret = (xsltNsMapPtr) xmlMalloc(sizeof(xsltNsMap));
    if (ret == NULL) {
	xsltTransformError(NULL, cctxt->style, elem,
	    "Internal error: (xsltNewNamespaceMapItem) "
	    "memory allocation failed.\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(xsltNsMap));
    ret->doc = doc;
    ret->ns = ns;
    ret->origNsName = ns->href;
     
    if (cctxt->psData->nsMap != NULL)
	ret->next = cctxt->psData->nsMap;
    cctxt->psData->nsMap = ret;

    return(ret);
}
