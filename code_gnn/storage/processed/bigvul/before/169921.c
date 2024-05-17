xsltCopyNamespace(xsltTransformContextPtr ctxt ATTRIBUTE_UNUSED,
		  xmlNodePtr elem, xmlNsPtr ns)
{
    if ((ns == NULL) || (ns->type != XML_NAMESPACE_DECL))
	return(NULL);
     
    if ((elem != NULL) && (elem->type != XML_ELEMENT_NODE))
	return(xmlNewNs(NULL, ns->href, ns->prefix));
    else
	return(xmlNewNs(elem, ns->href, ns->prefix));
}
