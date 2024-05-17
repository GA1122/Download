xsltRegisterGlobalVariable(xsltStylesheetPtr style, const xmlChar *name,
		     const xmlChar *ns_uri, const xmlChar *sel,
		     xmlNodePtr tree, xsltStylePreCompPtr comp,
		     const xmlChar *value) {
    xsltStackElemPtr elem, tmp;
    if (style == NULL)
	return(-1);
    if (name == NULL)
	return(-1);
    if (comp == NULL)
	return(-1);

#ifdef WITH_XSLT_DEBUG_VARIABLE
    if (comp->type == XSLT_FUNC_PARAM)
	xsltGenericDebug(xsltGenericDebugContext,
			 "Defining global param %s\n", name);
    else
	xsltGenericDebug(xsltGenericDebugContext,
			 "Defining global variable %s\n", name);
#endif

    elem = xsltNewStackElem(NULL);
    if (elem == NULL)
	return(-1);
    elem->comp = comp;
    elem->name = xmlDictLookup(style->dict, name, -1);
    elem->select = xmlDictLookup(style->dict, sel, -1);
    if (ns_uri)
	elem->nameURI = xmlDictLookup(style->dict, ns_uri, -1);
    elem->tree = tree;
    tmp = style->variables;
    if (tmp == NULL) {
	elem->next = NULL;
	style->variables = elem;
    } else {
	while (tmp != NULL) {
	    if ((elem->comp->type == XSLT_FUNC_VARIABLE) &&
		(tmp->comp->type == XSLT_FUNC_VARIABLE) &&
		(xmlStrEqual(elem->name, tmp->name)) &&
		((elem->nameURI == tmp->nameURI) ||
		 (xmlStrEqual(elem->nameURI, tmp->nameURI))))
	    {
		xsltTransformError(NULL, style, comp->inst,
		"redefinition of global variable %s\n", elem->name);
		style->errors++;
	    }
	    if (tmp->next == NULL)
	        break;
	    tmp = tmp->next;
	}
	elem->next = NULL;
	tmp->next = elem;
    }
    if (value != NULL) {
	elem->computed = 1;
	elem->value = xmlXPathNewString(value);
    }
    return(0);
}
