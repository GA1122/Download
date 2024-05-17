exsltFuncStyleShutdown (xsltStylesheetPtr style ATTRIBUTE_UNUSED,
			const xmlChar *URI ATTRIBUTE_UNUSED,
			xmlHashTablePtr data) {
    xmlHashFree(data, (xmlHashDeallocator) xmlFree);
}
