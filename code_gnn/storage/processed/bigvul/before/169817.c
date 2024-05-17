exsltFuncStyleInit (xsltStylesheetPtr style ATTRIBUTE_UNUSED,
		    const xmlChar *URI ATTRIBUTE_UNUSED) {
    return xmlHashCreate(1);
}
