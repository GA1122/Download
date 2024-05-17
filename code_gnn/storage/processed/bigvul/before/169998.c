xsltGetHTMLIDs(const xmlChar *version, const xmlChar **publicID,
	            const xmlChar **systemID) {
    unsigned int i;
    if (version == NULL)
	return(-1);
    for (i = 0;i < (sizeof(xsltHTMLVersions)/sizeof(xsltHTMLVersions[1]));
	 i++) {
	if (!xmlStrcasecmp(version,
		           (const xmlChar *) xsltHTMLVersions[i].version)) {
	    if (publicID != NULL)
		*publicID = (const xmlChar *) xsltHTMLVersions[i].public;
	    if (systemID != NULL)
		*systemID = (const xmlChar *) xsltHTMLVersions[i].system;
	    return(0);
	}
    }
    return(-1);
}
