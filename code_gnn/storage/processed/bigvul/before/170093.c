xsltParseNsPrefixList(xsltCompilerCtxtPtr cctxt,
		      xsltPointerListPtr list,
		      xmlNodePtr node,
		      const xmlChar *value)
{
    xmlChar *cur, *end;
    xmlNsPtr ns;

    if ((cctxt == NULL) || (value == NULL) || (list == NULL))
	return(-1);

    list->number = 0;

    cur = (xmlChar *) value;
    while (*cur != 0) {
	while (IS_BLANK(*cur)) cur++;
	if (*cur == 0)
	    break;
	end = cur;
	while ((*end != 0) && (!IS_BLANK(*end))) end++;
	cur = xmlStrndup(cur, end - cur);
	if (cur == NULL) {
	    cur = end;
	    continue;
	}
	 
	if ((cur[0] == '#') &&
	    xmlStrEqual(cur, (const xmlChar *)"#default"))
	    ns = xmlSearchNs(cctxt->style->doc, node, NULL);
	else
	    ns = xmlSearchNs(cctxt->style->doc, node, cur);

	if (ns == NULL) {
	     
	    xsltTransformError(NULL, cctxt->style, node,
		"No namespace binding in scope for prefix '%s'.\n", cur);
	     
	    cctxt->style->errors++;
	} else {
#ifdef WITH_XSLT_DEBUG_PARSING
	    xsltGenericDebug(xsltGenericDebugContext,
		"resolved prefix '%s'\n", cur);
#endif
	     
	    if (xsltPointerListAddSize(list,
		(void *) xmlDictLookup(cctxt->style->dict,
		ns->href, -1), 5) == -1)
	    {
		xmlFree(cur);
		goto internal_err;
	    }
	}
	xmlFree(cur);

	cur = end;
    }
    return(0);

internal_err:
    cctxt->style->errors++;
    return(-1);
}
