xmlXPathCastNodeToString (xmlNodePtr node) {
xmlChar *ret;
    if ((ret = xmlNodeGetContent(node)) == NULL)
	ret = xmlStrdup((const xmlChar *) "");
    return(ret);
}
