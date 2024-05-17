xsltNamespaceAlias(xsltStylesheetPtr style, xmlNodePtr node)
{
    xmlChar *resultPrefix = NULL;
    xmlChar *stylePrefix = NULL;
    xmlNsPtr literalNs = NULL;
    xmlNsPtr targetNs = NULL;

#ifdef XSLT_REFACTORED
    xsltNsAliasPtr alias;

    if ((style == NULL) || (node == NULL))
	return;

     
     
     
    stylePrefix = xmlGetNsProp(node, (const xmlChar *)"stylesheet-prefix", NULL);
    if (stylePrefix == NULL) {
	xsltTransformError(NULL, style, node,
	    "The attribute 'stylesheet-prefix' is missing.\n");
	return;
    }
    if (xmlStrEqual(stylePrefix, (const xmlChar *)"#default"))
	literalNs = xmlSearchNs(node->doc, node, NULL);
    else {
	literalNs = xmlSearchNs(node->doc, node, stylePrefix);
	if (literalNs == NULL) {
	    xsltTransformError(NULL, style, node,
	        "Attribute 'stylesheet-prefix': There's no namespace "
		"declaration in scope for the prefix '%s'.\n",
		    stylePrefix);
	    goto error;
	}
    }
     
    resultPrefix = xmlGetNsProp(node, (const xmlChar *)"result-prefix", NULL);
    if (resultPrefix == NULL) {
	xsltTransformError(NULL, style, node,
	    "The attribute 'result-prefix' is missing.\n");
	goto error;
    }
    if (xmlStrEqual(resultPrefix, (const xmlChar *)"#default"))
	targetNs = xmlSearchNs(node->doc, node, NULL);
    else {
	targetNs = xmlSearchNs(node->doc, node, resultPrefix);

        if (targetNs == NULL) {
	   xsltTransformError(NULL, style, node,
	        "Attribute 'result-prefix': There's no namespace "
		"declaration in scope for the prefix '%s'.\n",
		    stylePrefix);
	    goto error;
	}
    }
     


      
    alias = xsltNewNsAlias(XSLT_CCTXT(style));
    if (alias == NULL)
	return;
    alias->literalNs = literalNs;
    alias->targetNs = targetNs;
    XSLT_CCTXT(style)->hasNsAliases = 1;


#else  
    const xmlChar *literalNsName;
    const xmlChar *targetNsName;


    if ((style == NULL) || (node == NULL))
	return;

    stylePrefix = xmlGetNsProp(node, (const xmlChar *)"stylesheet-prefix", NULL);
    if (stylePrefix == NULL) {
	xsltTransformError(NULL, style, node,
	    "namespace-alias: stylesheet-prefix attribute missing\n");
	return;
    }
    resultPrefix = xmlGetNsProp(node, (const xmlChar *)"result-prefix", NULL);
    if (resultPrefix == NULL) {
	xsltTransformError(NULL, style, node,
	    "namespace-alias: result-prefix attribute missing\n");
	goto error;
    }

    if (xmlStrEqual(stylePrefix, (const xmlChar *)"#default")) {
	literalNs = xmlSearchNs(node->doc, node, NULL);
	if (literalNs == NULL) {
	    literalNsName = NULL;
	} else
	    literalNsName = literalNs->href;  
    } else {
	literalNs = xmlSearchNs(node->doc, node, stylePrefix);

	if ((literalNs == NULL) || (literalNs->href == NULL)) {
	    xsltTransformError(NULL, style, node,
	        "namespace-alias: prefix %s not bound to any namespace\n",
					stylePrefix);
	    goto error;
	} else
	    literalNsName = literalNs->href;
    }

     
    if (xmlStrEqual(resultPrefix, (const xmlChar *)"#default")) {
	targetNs = xmlSearchNs(node->doc, node, NULL);
	if (targetNs == NULL) {
	    targetNsName = UNDEFINED_DEFAULT_NS;
	} else
	    targetNsName = targetNs->href;
    } else {
	targetNs = xmlSearchNs(node->doc, node, resultPrefix);

        if ((targetNs == NULL) || (targetNs->href == NULL)) {
	    xsltTransformError(NULL, style, node,
	        "namespace-alias: prefix %s not bound to any namespace\n",
					resultPrefix);
	    goto error;
	} else
	    targetNsName = targetNs->href;
    }
     
    if (literalNsName == NULL) {
        if (targetNs != NULL) {
	     
            style->defaultAlias = targetNs->href;
	}
    } else {
        if (style->nsAliases == NULL)
	    style->nsAliases = xmlHashCreate(10);
        if (style->nsAliases == NULL) {
	    xsltTransformError(NULL, style, node,
	        "namespace-alias: cannot create hash table\n");
	    goto error;
        }
	xmlHashAddEntry((xmlHashTablePtr) style->nsAliases,
	    literalNsName, (void *) targetNsName);
    }
#endif  

error:
    if (stylePrefix != NULL)
	xmlFree(stylePrefix);
    if (resultPrefix != NULL)
	xmlFree(resultPrefix);
}
