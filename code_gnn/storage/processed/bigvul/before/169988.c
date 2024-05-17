xsltCopyAttrListNoOverwrite(xsltTransformContextPtr ctxt,
			    xmlNodePtr invocNode,
			    xmlNodePtr target, xmlAttrPtr attr)
{
    xmlAttrPtr copy;
    xmlNsPtr origNs = NULL, copyNs = NULL;
    xmlChar *value;

     
    while (attr != NULL) {
	 
	if (attr->ns != origNs) {
	    origNs = attr->ns;
	    if (attr->ns != NULL) {
		copyNs = xsltGetSpecialNamespace(ctxt, invocNode,
		    attr->ns->href, attr->ns->prefix, target);
		if (copyNs == NULL)
		    return(-1);
	    } else
		copyNs = NULL;
	}
	 
	if ((attr->children) && (attr->children->type == XML_TEXT_NODE) &&
            (attr->children->next == NULL)) {
            copy = xmlNewNsProp(target, copyNs, attr->name,
                                attr->children->content);
        } else if (attr->children != NULL) {
	    value = xmlNodeListGetString(attr->doc, attr->children, 1);
            copy = xmlNewNsProp(target, copyNs, attr->name, BAD_CAST value);
	    xmlFree(value);
        } else {
            copy = xmlNewNsProp(target, copyNs, attr->name, NULL);
        }

	if (copy == NULL)
	    return(-1);

	attr = attr->next;
    }
    return(0);
}
