xmlXPathNodeSetDupNs(xmlNodePtr node, xmlNsPtr ns) {
    xmlNsPtr cur;

    if ((ns == NULL) || (ns->type != XML_NAMESPACE_DECL))
	return(NULL);
    if ((node == NULL) || (node->type == XML_NAMESPACE_DECL))
	return((xmlNodePtr) ns);

     
    cur = (xmlNsPtr) xmlMalloc(sizeof(xmlNs));
    if (cur == NULL) {
        xmlXPathErrMemory(NULL, "duplicating namespace\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNs));
    cur->type = XML_NAMESPACE_DECL;
    if (ns->href != NULL)
	cur->href = xmlStrdup(ns->href);
    if (ns->prefix != NULL)
	cur->prefix = xmlStrdup(ns->prefix);
    cur->next = (xmlNsPtr) node;
    return((xmlNodePtr) cur);
}
