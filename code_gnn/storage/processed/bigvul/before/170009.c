xsltShallowCopyAttr(xsltTransformContextPtr ctxt, xmlNodePtr invocNode,
	     xmlNodePtr target, xmlAttrPtr attr)
{
    xmlAttrPtr copy;
    xmlChar *value;

    if (attr == NULL)
	return(NULL);

    if (target->type != XML_ELEMENT_NODE) {
	xsltTransformError(ctxt, NULL, invocNode,
	    "Cannot add an attribute node to a non-element node.\n");
	return(NULL);
    }

    if (target->children != NULL) {
	xsltTransformError(ctxt, NULL, invocNode,
	    "Attribute nodes must be added before "
	    "any child nodes to an element.\n");
	return(NULL);
    }

    value = xmlNodeListGetString(attr->doc, attr->children, 1);
    if (attr->ns != NULL) {
	xmlNsPtr ns;

	ns = xsltGetSpecialNamespace(ctxt, invocNode,
	    attr->ns->href, attr->ns->prefix, target);
	if (ns == NULL) {
	    xsltTransformError(ctxt, NULL, invocNode,
		"Namespace fixup error: Failed to acquire an in-scope "
		"namespace binding of the copied attribute '{%s}%s'.\n",
		attr->ns->href, attr->name);
	     
	}
	 
	copy = xmlSetNsProp(target, ns, attr->name, value);
    } else {
	copy = xmlSetNsProp(target, NULL, attr->name, value);
    }
    if (value != NULL)
	xmlFree(value);

    if (copy == NULL)
	return(NULL);

#if 0
     
     
    value = xmlNodeListGetString(attr->doc, attr->children, 1);
    if (value != NULL) {
	txtNode = xmlNewDocText(target->doc, NULL);
	if (txtNode == NULL)
	    return(NULL);
	if ((target->doc != NULL) &&
	    (target->doc->dict != NULL))
	{
	    txtNode->content =
		(xmlChar *) xmlDictLookup(target->doc->dict,
		    BAD_CAST value, -1);
	    xmlFree(value);
	} else
	    txtNode->content = value;
	copy->children = txtNode;
    }
#endif

    return(copy);
}
