xsltCheckStackElem(xsltTransformContextPtr ctxt, const xmlChar *name,
	           const xmlChar *nameURI) {
    xsltStackElemPtr cur;

    if ((ctxt == NULL) || (name == NULL))
	return(-1);

    cur = xsltStackLookup(ctxt, name, nameURI);
    if (cur == NULL)
        return(0);
    if (cur->comp != NULL) {
        if (cur->comp->type == XSLT_FUNC_WITHPARAM)
	    return(3);
	else if (cur->comp->type == XSLT_FUNC_PARAM)
	    return(2);
    }

    return(1);
}
