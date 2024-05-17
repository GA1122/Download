xsltLoadStylesheetPI(xmlDocPtr doc) {
    xmlNodePtr child;
    xsltStylesheetPtr ret = NULL;
    xmlChar *href = NULL;
    xmlURIPtr URI;

    xsltInitGlobals();

    if (doc == NULL)
	return(NULL);

     
    child = doc->children;
    while ((child != NULL) && (child->type != XML_ELEMENT_NODE)) {
	if ((child->type == XML_PI_NODE) &&
	    (xmlStrEqual(child->name, BAD_CAST "xml-stylesheet"))) {
	    href = xsltParseStylesheetPI(child->content);
	    if (href != NULL)
		break;
	}
	child = child->next;
    }

     
    if (href != NULL) {
#ifdef WITH_XSLT_DEBUG_PARSING
	xsltGenericDebug(xsltGenericDebugContext,
		"xsltLoadStylesheetPI : found PI href=%s\n", href);
#endif
	URI = xmlParseURI((const char *) href);
	if (URI == NULL) {
	    xsltTransformError(NULL, NULL, child,
		    "xml-stylesheet : href %s is not valid\n", href);
	    xmlFree(href);
	    return(NULL);
	}
	if ((URI->fragment != NULL) && (URI->scheme == NULL) &&
            (URI->opaque == NULL) && (URI->authority == NULL) &&
            (URI->server == NULL) && (URI->user == NULL) &&
            (URI->path == NULL) && (URI->query == NULL)) {
	    xmlAttrPtr ID;

#ifdef WITH_XSLT_DEBUG_PARSING
	    xsltGenericDebug(xsltGenericDebugContext,
		    "xsltLoadStylesheetPI : Reference to ID %s\n", href);
#endif
	    if (URI->fragment[0] == '#')
		ID = xmlGetID(doc, (const xmlChar *) &(URI->fragment[1]));
	    else
		ID = xmlGetID(doc, (const xmlChar *) URI->fragment);
	    if (ID == NULL) {
		xsltTransformError(NULL, NULL, child,
		    "xml-stylesheet : no ID %s found\n", URI->fragment);
	    } else {
		xmlDocPtr fake;
		xmlNodePtr subtree, newtree;
		xmlNsPtr ns;

#ifdef WITH_XSLT_DEBUG
		xsltGenericDebug(xsltGenericDebugContext,
		    "creating new document from %s for embedded stylesheet\n",
		    doc->URL);
#endif
		 
		subtree = ID->parent;
		fake = xmlNewDoc(NULL);
		if (fake != NULL) {
		     
		    fake->dict = doc->dict;
		    xmlDictReference(doc->dict);
#ifdef WITH_XSLT_DEBUG
		    xsltGenericDebug(xsltGenericDebugContext,
			"reusing dictionary from %s for embedded stylesheet\n",
			doc->URL);
#endif

		    newtree = xmlDocCopyNode(subtree, fake, 1);

		    fake->URL = xmlNodeGetBase(doc, subtree->parent);
#ifdef WITH_XSLT_DEBUG
		    xsltGenericDebug(xsltGenericDebugContext,
			"set base URI for embedded stylesheet as %s\n",
			fake->URL);
#endif

		     
		    while ((subtree = subtree->parent) != (xmlNodePtr)doc) {
			for (ns = subtree->ns; ns; ns = ns->next) {
			    xmlNewNs(newtree,  ns->href, ns->prefix);
			}
		    }

		    xmlAddChild((xmlNodePtr)fake, newtree);
		    ret = xsltParseStylesheetDoc(fake);
		    if (ret == NULL)
			xmlFreeDoc(fake);
		}
	    }
	} else {
	    xmlChar *URL, *base;

	     

	    base = xmlNodeGetBase(doc, (xmlNodePtr) doc);
	    URL = xmlBuildURI(href, base);
	    if (URL != NULL) {
#ifdef WITH_XSLT_DEBUG_PARSING
		xsltGenericDebug(xsltGenericDebugContext,
			"xsltLoadStylesheetPI : fetching %s\n", URL);
#endif
		ret = xsltParseStylesheetFile(URL);
		xmlFree(URL);
	    } else {
#ifdef WITH_XSLT_DEBUG_PARSING
		xsltGenericDebug(xsltGenericDebugContext,
			"xsltLoadStylesheetPI : fetching %s\n", href);
#endif
		ret = xsltParseStylesheetFile(href);
	    }
	    if (base != NULL)
		xmlFree(base);
	}
	xmlFreeURI(URI);
	xmlFree(href);
    }
    return(ret);
}
