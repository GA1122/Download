xsltGetSpecialNamespace(xsltTransformContextPtr ctxt, xmlNodePtr invocNode,
		const xmlChar *nsName, const xmlChar *nsPrefix,
		xmlNodePtr target)
{
    xmlNsPtr ns;
    int prefixOccupied = 0;

    if ((ctxt == NULL) || (target == NULL) ||
	(target->type != XML_ELEMENT_NODE))
	return(NULL);

     
     
    if ((nsPrefix == NULL) && ((nsName == NULL) || (nsName[0] == 0))) {
	 
	 
	 
	if (target->nsDef != NULL) {
	    ns = target->nsDef;
	    do {
		if (ns->prefix == NULL) {
		    if ((ns->href != NULL) && (ns->href[0] != 0)) {
			 
			xsltTransformError(ctxt, NULL, invocNode,
			    "Namespace normalization error: Cannot undeclare "
			    "the default namespace, since the default namespace "
			    "'%s' is already declared on the result element "
			    "'%s'.\n", ns->href, target->name);
			return(NULL);
		    } else {
			 
			return(NULL);
		    }
		    break;
		}
		ns = ns->next;
	    } while (ns != NULL);
	}
	if ((target->parent != NULL) &&
	    (target->parent->type == XML_ELEMENT_NODE))
	{
	     
	    if (target->parent->ns == NULL)
		return(NULL);

	    ns = xmlSearchNs(target->doc, target->parent,
		NULL);
	     
	    if ((ns == NULL) || (ns->href == NULL) || (ns->href[0] == 0))
		return(NULL);

	     
	    xmlNewNs(target, BAD_CAST "", NULL);
	     
	    return(NULL);
	}
	return(NULL);
    }
     
    if ((nsPrefix != NULL) &&
	(nsPrefix[0] == 'x') && (nsPrefix[1] == 'm') &&
	(nsPrefix[2] == 'l') && (nsPrefix[3] == 0))
    {
	return(xmlSearchNs(target->doc, target, nsPrefix));
    }
     
    if (target->nsDef != NULL) {
	ns = target->nsDef;
	do {
	    if ((ns->prefix == NULL) == (nsPrefix == NULL)) {
		if (ns->prefix == nsPrefix) {
		    if (xmlStrEqual(ns->href, nsName))
			return(ns);
		    prefixOccupied = 1;
		    break;
		} else if (xmlStrEqual(ns->prefix, nsPrefix)) {
		    if (xmlStrEqual(ns->href, nsName))
			return(ns);
		    prefixOccupied = 1;
		    break;
		}
	    }
	    ns = ns->next;
	} while (ns != NULL);
    }
    if (prefixOccupied) {
	 
	ns = xmlSearchNsByHref(target->doc, target, nsName);
	if (ns != NULL)
	    return(ns);

	 
    } else if ((target->parent != NULL) &&
	(target->parent->type == XML_ELEMENT_NODE))
    {
	 
	if ((target->parent->ns != NULL) &&
	    ((target->parent->ns->prefix != NULL) == (nsPrefix != NULL)))
	{
	    ns = target->parent->ns;

	    if (nsPrefix == NULL) {
		if (xmlStrEqual(ns->href, nsName))
		    return(ns);
	    } else if (xmlStrEqual(ns->prefix, nsPrefix) &&
		xmlStrEqual(ns->href, nsName))
	    {
		return(ns);
	    }
	}
	 
	ns = xmlSearchNs(target->doc, target->parent, nsPrefix);
	if (ns != NULL) {
	    if (xmlStrEqual(ns->href, nsName))
		return(ns);
	     
	    if (target->properties) {
		xmlAttrPtr attr = target->properties;
		do {
		    if ((attr->ns) &&
			xmlStrEqual(attr->ns->prefix, nsPrefix))
		    {
			 
			ns = xmlSearchNsByHref(target->doc, target, nsName);
			if (ns != NULL)
			    return(ns);
			goto declare_new_prefix;
		    }
		    attr = attr->next;
		} while (attr != NULL);
	    }
	} else {
	     
#if 0
	    ns = xmlSearchNsByHref(target->doc, target, nsName);
	    if (ns != NULL)
		return(ns);
#endif
	}
	 
	ns = xmlNewNs(target, nsName, nsPrefix);
	 
	return(ns);
    } else {
	 
	ns = xmlNewNs(target, nsName, nsPrefix);
	 
	return(ns);
    }

declare_new_prefix:
     
    {
	xmlChar pref[30];
	int counter = 1;

	if (nsPrefix == NULL) {
	    nsPrefix = BAD_CAST "ns";
	}

	do {
	    snprintf((char *) pref, 30, "%s_%d", nsPrefix, counter++);
	    ns = xmlSearchNs(target->doc, target, BAD_CAST pref);
	    if (counter > 1000) {
		xsltTransformError(ctxt, NULL, invocNode,
		    "Internal error in xsltAcquireResultInScopeNs(): "
		    "Failed to compute a unique ns-prefix for the "
		    "generated element");
		return(NULL);
	    }
	} while (ns != NULL);
	ns = xmlNewNs(target, nsName, BAD_CAST pref);
	 
	return(ns);
    }
    return(NULL);
}
