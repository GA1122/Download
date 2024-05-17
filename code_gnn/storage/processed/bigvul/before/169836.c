xsltResolveStylesheetAttributeSet(xsltStylesheetPtr style) {
    xsltStylesheetPtr cur;

#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
    xsltGenericDebug(xsltGenericDebugContext,
	    "Resolving attribute sets references\n");
#endif
     
    cur = xsltNextImport(style);
    while (cur != NULL) {
	if (cur->attributeSets != NULL) {
	    if (style->attributeSets == NULL) {
#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
		xsltGenericDebug(xsltGenericDebugContext,
		    "creating attribute set table\n");
#endif
		style->attributeSets = xmlHashCreate(10);
	    }
	    xmlHashScanFull(cur->attributeSets,
		(xmlHashScannerFull) xsltMergeSASCallback, style);
	     
	    xmlHashFree(cur->attributeSets, NULL);
	    cur->attributeSets = NULL;
	}
	cur = xsltNextImport(cur);
    }

     
    if (style->attributeSets != NULL) {
	xmlHashScanFull(style->attributeSets,
		(xmlHashScannerFull) xsltResolveSASCallback, style);
    }
}
