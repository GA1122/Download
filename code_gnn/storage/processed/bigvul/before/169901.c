xsltFindElemSpaceHandling(xsltTransformContextPtr ctxt, xmlNodePtr node) {
    xsltStylesheetPtr style;
    const xmlChar *val;

    if ((ctxt == NULL) || (node == NULL))
	return(0);
    style = ctxt->style;
    while (style != NULL) {
	if (node->ns != NULL) {
	    val = (const xmlChar *)
	      xmlHashLookup2(style->stripSpaces, node->name, node->ns->href);
            if (val == NULL) {
                val = (const xmlChar *)
                    xmlHashLookup2(style->stripSpaces, BAD_CAST "*",
                                   node->ns->href);
            }
	} else {
	    val = (const xmlChar *)
		  xmlHashLookup2(style->stripSpaces, node->name, NULL);
	}
	if (val != NULL) {
	    if (xmlStrEqual(val, (xmlChar *) "strip"))
		return(1);
	    if (xmlStrEqual(val, (xmlChar *) "preserve"))
		return(0);
	}
	if (style->stripAll == 1)
	    return(1);
	if (style->stripAll == -1)
	    return(0);

	style = xsltNextImport(style);
    }
    return(0);
}
