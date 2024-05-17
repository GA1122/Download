xsltLREBuildEffectiveNsNodes(xsltCompilerCtxtPtr cctxt,
			     xsltStyleItemLRElementInfoPtr item,
			     xmlNodePtr elem,
			     int isLRE)
{
    xmlNsPtr ns, tmpns;
    xsltEffectiveNsPtr effNs, lastEffNs = NULL;
    int i, j, holdByElem;
    xsltPointerListPtr extElemNs = cctxt->inode->extElemNs;
    xsltPointerListPtr exclResultNs = cctxt->inode->exclResultNs;

    if ((cctxt == NULL) || (cctxt->inode == NULL) || (elem == NULL) ||
	(item == NULL) || (item->effectiveNs != NULL))
	return(-1);

    if (item->inScopeNs == NULL)
	return(0);

    extElemNs = cctxt->inode->extElemNs;
    exclResultNs = cctxt->inode->exclResultNs;

    for (i = 0; i < item->inScopeNs->totalNumber; i++) {
	ns = item->inScopeNs->list[i];
	 
	if (xmlStrEqual(ns->href, XSLT_NAMESPACE))
	    goto skip_ns;

	 
	if (cctxt->hasNsAliases) {
	    xsltNsAliasPtr alias;
	     
	    alias = cctxt->nsAliases;
	    do {
		 
		if ((alias->targetNs != NULL) &&
		    (xmlStrEqual(alias->targetNs->href, ns->href)))
		{
		     
		    goto add_effective_ns;
		}
		alias = alias->next;
	    } while (alias != NULL);

	    alias = cctxt->nsAliases;
	    do {
		 
		if ((alias->literalNs != NULL) &&
		    (xmlStrEqual(alias->literalNs->href, ns->href)))
		{
		     
		    goto skip_ns;
		}
		alias = alias->next;
	    } while (alias != NULL);
	}

	 
	if (exclResultNs) {
	    for (j = 0; j < exclResultNs->number; j++)
		if (xmlStrEqual(ns->href, BAD_CAST exclResultNs->items[j]))
		    goto skip_ns;
	}
	 
	if (extElemNs) {
	    for (j = 0; j < extElemNs->number; j++)
		if (xmlStrEqual(ns->href, BAD_CAST extElemNs->items[j]))
		    goto skip_ns;
	}

add_effective_ns:
	 
	if (isLRE && (elem->nsDef != NULL)) {
	    holdByElem = 0;
	    tmpns = elem->nsDef;
	    do {
		if (tmpns == ns) {
		    holdByElem = 1;
		    break;
		}
		tmpns = tmpns->next;
	    } while (tmpns != NULL);
	} else
	    holdByElem = 0;


	 
	effNs = (xsltEffectiveNsPtr) xmlMalloc(sizeof(xsltEffectiveNs));
	if (effNs == NULL) {
	    xsltTransformError(NULL, cctxt->style, elem,
		"Internal error in xsltLREBuildEffectiveNs(): "
		"failed to allocate memory.\n");
	    cctxt->style->errors++;
	    return(-1);
	}
	if (cctxt->psData->effectiveNs == NULL) {
	    cctxt->psData->effectiveNs = effNs;
	    effNs->nextInStore = NULL;
	} else {
	    effNs->nextInStore = cctxt->psData->effectiveNs;
	    cctxt->psData->effectiveNs = effNs;
	}

	effNs->next = NULL;
	effNs->prefix = ns->prefix;
	effNs->nsName = ns->href;
	effNs->holdByElem = holdByElem;

	if (lastEffNs == NULL)
	    item->effectiveNs = effNs;
	else
	    lastEffNs->next = effNs;
	lastEffNs = effNs;

skip_ns:
	{}
    }
    return(0);
}
