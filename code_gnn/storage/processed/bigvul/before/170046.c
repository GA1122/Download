xsltStackLookup(xsltTransformContextPtr ctxt, const xmlChar *name,
	        const xmlChar *nameURI) {
    int i;
    xsltStackElemPtr cur;

    if ((ctxt == NULL) || (name == NULL) || (ctxt->varsNr == 0))
	return(NULL);

     
    for (i = ctxt->varsNr; i > ctxt->varsBase; i--) {
	cur = ctxt->varsTab[i-1];
	while (cur != NULL) {
	    if ((cur->name == name) && (cur->nameURI == nameURI)) {
#if 0
		stack_addr++;
#endif
		return(cur);
	    }
	    cur = cur->next;
	}
    }

     
    name = xmlDictLookup(ctxt->dict, name, -1);
    if (nameURI != NULL)
        nameURI = xmlDictLookup(ctxt->dict, nameURI, -1);

    for (i = ctxt->varsNr; i > ctxt->varsBase; i--) {
	cur = ctxt->varsTab[i-1];
	while (cur != NULL) {
	    if ((cur->name == name) && (cur->nameURI == nameURI)) {
#if 0
		stack_cmp++;
#endif
		return(cur);
	    }
	    cur = cur->next;
	}
    }

    return(NULL);
}
