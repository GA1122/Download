xsltCopyNamespaceListInternal(xmlNodePtr elem, xmlNsPtr ns) {
    xmlNsPtr ret = NULL;
    xmlNsPtr p = NULL, q, luNs;

    if (ns == NULL)
	return(NULL);
     
    if ((elem != NULL) && (elem->type != XML_ELEMENT_NODE))
	elem = NULL;

    do {
	if (ns->type != XML_NAMESPACE_DECL)
	    break;
	 
	if (elem != NULL) {
	    if ((elem->ns != NULL) &&
		xmlStrEqual(elem->ns->prefix, ns->prefix) &&
		xmlStrEqual(elem->ns->href, ns->href))
	    {
		ns = ns->next;
		continue;
	    }
	    luNs = xmlSearchNs(elem->doc, elem, ns->prefix);
	    if ((luNs != NULL) && (xmlStrEqual(luNs->href, ns->href)))
	    {
		ns = ns->next;
		continue;
	    }
	}
	q = xmlNewNs(elem, ns->href, ns->prefix);
	if (p == NULL) {
	    ret = p = q;
	} else if (q != NULL) {
	    p->next = q;
	    p = q;
	}
	ns = ns->next;
    } while (ns != NULL);
    return(ret);
}
