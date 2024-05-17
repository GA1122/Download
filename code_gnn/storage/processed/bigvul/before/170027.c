xsltExtensionInstructionResultRegister(xsltTransformContextPtr ctxt,
				       xmlXPathObjectPtr obj)
{
    int i;
    xmlNodePtr cur;
    xmlDocPtr doc;

    if ((ctxt == NULL) || (obj == NULL))
	return(-1);

     

    if ((obj->type != XPATH_NODESET) && (obj->type != XPATH_XSLT_TREE))
	return(0);
    if ((obj->nodesetval == NULL) || (obj->nodesetval->nodeNr == 0))
	return(0);

    for (i = 0; i < obj->nodesetval->nodeNr; i++) {
	cur = obj->nodesetval->nodeTab[i];
	if (cur->type == XML_NAMESPACE_DECL) {
	     
	    if ((((xmlNsPtr) cur)->next != NULL) &&
		(((xmlNsPtr) cur)->next->type == XML_ELEMENT_NODE))
	    {
		cur = (xmlNodePtr) ((xmlNsPtr) cur)->next;
		doc = cur->doc;
	    } else {
		xsltTransformError(ctxt, NULL, ctxt->inst,
		    "Internal error in "
		    "xsltExtensionInstructionResultRegister(): "
		    "Cannot retrieve the doc of a namespace node.\n");
		goto error;
	    }
	} else {
	    doc = cur->doc;
	}
	if (doc == NULL) {
	    xsltTransformError(ctxt, NULL, ctxt->inst,
		"Internal error in "
		"xsltExtensionInstructionResultRegister(): "
		"Cannot retrieve the doc of a node.\n");
	    goto error;
	}
	if (doc->name && (doc->name[0] == ' ')) {
	     
	    doc->psvi = (void *) ((long) 1);
	}
    }

    return(0);
error:
    return(-1);
}
