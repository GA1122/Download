xsltMergeSASCallback(xsltAttrElemPtr values, xsltStylesheetPtr style,
	               const xmlChar *name, const xmlChar *ns,
		       ATTRIBUTE_UNUSED const xmlChar *ignored) {
    int ret;
    xsltAttrElemPtr topSet;

    ret = xmlHashAddEntry2(style->attributeSets, name, ns, values);
    if (ret < 0) {
	 
#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
	xsltGenericDebug(xsltGenericDebugContext,
		"attribute set %s present already in top stylesheet"
		" - merging\n", name);
#endif
	topSet = xmlHashLookup2(style->attributeSets, name, ns);
	if (topSet==NULL) {
	    xsltGenericError(xsltGenericErrorContext,
	        "xsl:attribute-set : logic error merging from imports for"
		" attribute-set %s\n", name);
	} else {
	    topSet = xsltMergeAttrElemList(style, topSet, values);
	    xmlHashUpdateEntry2(style->attributeSets, name, ns, topSet, NULL);
	}
	xsltFreeAttrElemList(values);
#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
    } else {
	xsltGenericDebug(xsltGenericDebugContext,
		"attribute set %s moved to top stylesheet\n",
		         name);
#endif
    }
}
