xsltGetNamespace(xsltTransformContextPtr ctxt, xmlNodePtr cur, xmlNsPtr ns,
	         xmlNodePtr out)
{

    if (ns == NULL)
	return(NULL);

#ifdef XSLT_REFACTORED
     
    return(xsltGetSpecialNamespace(ctxt, cur, ns->href, ns->prefix, out));
#else
    {
	xsltStylesheetPtr style;
	const xmlChar *URI = NULL;  

	if ((ctxt == NULL) || (cur == NULL) || (out == NULL))
	    return(NULL);

	style = ctxt->style;
	while (style != NULL) {
	    if (style->nsAliases != NULL)
		URI = (const xmlChar *)
		xmlHashLookup(style->nsAliases, ns->href);
	    if (URI != NULL)
		break;

	    style = xsltNextImport(style);
	}


	if (URI == UNDEFINED_DEFAULT_NS) {
	    return(xsltGetSpecialNamespace(ctxt, cur, NULL, NULL, out));
#if 0
	     
	    xmlNsPtr dflt;
	    dflt = xmlSearchNs(cur->doc, cur, NULL);
	    if (dflt != NULL)
		URI = dflt->href;
	    else
		return NULL;
#endif
	} else if (URI == NULL)
	    URI = ns->href;

	return(xsltGetSpecialNamespace(ctxt, cur, URI, ns->prefix, out));
    }
#endif
}
