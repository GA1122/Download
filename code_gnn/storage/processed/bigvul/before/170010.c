xsltShallowCopyNsNode(xsltTransformContextPtr ctxt,
		      xmlNodePtr invocNode,
		      xmlNodePtr insert,
		      xmlNsPtr ns)
{
     
    xmlNsPtr tmpns;

    if ((insert == NULL) || (insert->type != XML_ELEMENT_NODE))
	return(NULL);

    if (insert->children != NULL) {
	xsltTransformError(ctxt, NULL, invocNode,
	    "Namespace nodes must be added before "
	    "any child nodes are added to an element.\n");
	return(NULL);
    }
     
    if (ns->prefix == NULL) {
	 
	if (insert->ns == NULL)
	    goto occupied;
    } else if ((ns->prefix[0] == 'x') &&
	xmlStrEqual(ns->prefix, BAD_CAST "xml"))
    {
	 
	return(NULL);
    }

    if (insert->nsDef != NULL) {
	tmpns = insert->nsDef;
	do {
	    if ((tmpns->prefix == NULL) == (ns->prefix == NULL)) {
		if ((tmpns->prefix == ns->prefix) ||
		    xmlStrEqual(tmpns->prefix, ns->prefix))
		{
		     
		    if (xmlStrEqual(tmpns->href, ns->href))
			return(NULL);
		    goto occupied;
		}
	    }
	    tmpns = tmpns->next;
	} while (tmpns != NULL);
    }
    tmpns = xmlSearchNs(insert->doc, insert, ns->prefix);
    if ((tmpns != NULL) && xmlStrEqual(tmpns->href, ns->href))
	return(NULL);
     
    return(xmlNewNs(insert, ns->href, ns->prefix));

occupied:
     
    return(NULL);
}
