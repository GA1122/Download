xsltParseStylesheetInclude(xsltStylesheetPtr style, xmlNodePtr cur) {
    int ret = -1;
    xmlDocPtr oldDoc;
    xmlChar *base = NULL;
    xmlChar *uriRef = NULL;
    xmlChar *URI = NULL;
    xsltStylesheetPtr result;
    xsltDocumentPtr include;
    xsltDocumentPtr docptr;
    int oldNopreproc;

    if ((cur == NULL) || (style == NULL))
	return (ret);

    uriRef = xmlGetNsProp(cur, (const xmlChar *)"href", NULL);
    if (uriRef == NULL) {
	xsltTransformError(NULL, style, cur,
	    "xsl:include : missing href attribute\n");
	goto error;
    }

    base = xmlNodeGetBase(style->doc, cur);
    URI = xmlBuildURI(uriRef, base);
    if (URI == NULL) {
	xsltTransformError(NULL, style, cur,
	    "xsl:include : invalid URI reference %s\n", uriRef);
	goto error;
    }

     
    docptr = style->includes;
    while (docptr != NULL) {
        if (xmlStrEqual(docptr->doc->URL, URI)) {
	    xsltTransformError(NULL, style, cur,
	        "xsl:include : recursion detected on included URL %s\n", URI);
	    goto error;
	}
	docptr = docptr->includes;
    }

    include = xsltLoadStyleDocument(style, URI);
    if (include == NULL) {
	xsltTransformError(NULL, style, cur,
	    "xsl:include : unable to load %s\n", URI);
	goto error;
    }
#ifdef XSLT_REFACTORED
    if (IS_XSLT_ELEM_FAST(cur) && (cur->psvi != NULL)) {
	((xsltStyleItemIncludePtr) cur->psvi)->include = include;
    } else {
	xsltTransformError(NULL, style, cur,
	    "Internal error: (xsltParseStylesheetInclude) "
	    "The xsl:include element was not compiled.\n", URI);
	style->errors++;
    }
#endif
    oldDoc = style->doc;
    style->doc = include->doc;
     
    include->includes = style->includes;
    style->includes = include;
    oldNopreproc = style->nopreproc;
    style->nopreproc = include->preproc;
     
    result = xsltParseStylesheetProcess(style, include->doc);
    style->nopreproc = oldNopreproc;
    include->preproc = 1;
    style->includes = include->includes;
    style->doc = oldDoc;
    if (result == NULL) {
	ret = -1;
	goto error;
    }
    ret = 0;

error:
    if (uriRef != NULL)
	xmlFree(uriRef);
    if (base != NULL)
	xmlFree(base);
    if (URI != NULL)
	xmlFree(URI);

    return (ret);
}
