xmlXPathTryStreamCompile(xmlXPathContextPtr ctxt, const xmlChar *str) {
     
    xmlPatternPtr stream;
    xmlXPathCompExprPtr comp;
    xmlDictPtr dict = NULL;
    const xmlChar **namespaces = NULL;
    xmlNsPtr ns;
    int i, j;

    if ((!xmlStrchr(str, '[')) && (!xmlStrchr(str, '(')) &&
        (!xmlStrchr(str, '@'))) {
	const xmlChar *tmp;

	 
	tmp = xmlStrchr(str, ':');
	if ((tmp != NULL) &&
	    ((ctxt == NULL) || (ctxt->nsNr == 0) || (tmp[1] == ':')))
	    return(NULL);

	if (ctxt != NULL) {
	    dict = ctxt->dict;
	    if (ctxt->nsNr > 0) {
		namespaces = xmlMalloc(2 * (ctxt->nsNr + 1) * sizeof(xmlChar*));
		if (namespaces == NULL) {
		    xmlXPathErrMemory(ctxt, "allocating namespaces array\n");
		    return(NULL);
		}
		for (i = 0, j = 0; (j < ctxt->nsNr); j++) {
		    ns = ctxt->namespaces[j];
		    namespaces[i++] = ns->href;
		    namespaces[i++] = ns->prefix;
		}
		namespaces[i++] = NULL;
		namespaces[i] = NULL;
	    }
	}

	stream = xmlPatterncompile(str, dict, XML_PATTERN_XPATH,
			&namespaces[0]);
	if (namespaces != NULL) {
	    xmlFree((xmlChar **)namespaces);
	}
	if ((stream != NULL) && (xmlPatternStreamable(stream) == 1)) {
	    comp = xmlXPathNewCompExpr();
	    if (comp == NULL) {
		xmlXPathErrMemory(ctxt, "allocating streamable expression\n");
		return(NULL);
	    }
	    comp->stream = stream;
	    comp->dict = dict;
	    if (comp->dict)
		xmlDictReference(comp->dict);
	    return(comp);
	}
	xmlFreePattern(stream);
    }
    return(NULL);
}
