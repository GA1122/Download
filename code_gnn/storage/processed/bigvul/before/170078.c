xsltLREBuildEffectiveNs(xsltCompilerCtxtPtr cctxt,
			xmlNodePtr elem)
{
    xmlNsPtr ns;
    xsltNsAliasPtr alias;

    if ((cctxt == NULL) || (elem == NULL))
	return(-1);
    if ((cctxt->nsAliases == NULL) || (! cctxt->hasNsAliases))
	return(0);

    alias = cctxt->nsAliases;
    while (alias != NULL) {
	if (  
	    ( (elem->ns == NULL) &&
	    ((alias->literalNs == NULL) ||
	    (alias->literalNs->href == NULL)) ) ||
	     
	    ( (elem->ns != NULL) &&
	    (alias->literalNs != NULL) &&
	    xmlStrEqual(elem->ns->href, alias->literalNs->href) ) )
	{
	    if ((alias->targetNs != NULL) &&
		(alias->targetNs->href != NULL))
	    {
		 
		if (elem->doc == alias->docOfTargetNs) {
		     
		    elem->ns = alias->targetNs;
		} else {
		     
		    ns = xmlSearchNs(elem->doc, elem,
			alias->targetNs->prefix);
		     
		    if ((ns == NULL) ||
			(! xmlStrEqual(ns->href, alias->targetNs->href)))
		    {
			 
			ns = xsltTreeAcquireStoredNs(elem->doc,
			    alias->targetNs->href,
			    alias->targetNs->prefix);

			if (ns == NULL) {
			    xsltTransformError(NULL, cctxt->style, elem,
				"Internal error in "
				"xsltLREBuildEffectiveNs(): "
				"failed to acquire a stored "
				"ns-declaration.\n");
			    cctxt->style->errors++;
			    return(-1);

			}
		    }
		    elem->ns = ns;
		}
	    } else {
		 
		elem->ns = NULL;
	    }
	    break;
	}
	alias = alias->next;
    }
     
    if (elem->properties != NULL) {
	xmlAttrPtr attr = elem->properties;

	while (attr != NULL) {
	    if (attr->ns == NULL) {
		attr = attr->next;
		continue;
	    }
	    alias = cctxt->nsAliases;
	    while (alias != NULL) {
		if (  
		    ( (elem->ns == NULL) &&
		    ((alias->literalNs == NULL) ||
		    (alias->literalNs->href == NULL)) ) ||
		     
		    ( (elem->ns != NULL) &&
		    (alias->literalNs != NULL) &&
		    xmlStrEqual(elem->ns->href, alias->literalNs->href) ) )
		{
		    if ((alias->targetNs != NULL) &&
			(alias->targetNs->href != NULL))
		    {
			if (elem->doc == alias->docOfTargetNs) {
			    elem->ns = alias->targetNs;
			} else {
			    ns = xmlSearchNs(elem->doc, elem,
				alias->targetNs->prefix);
			    if ((ns == NULL) ||
				(! xmlStrEqual(ns->href, alias->targetNs->href)))
			    {
				ns = xsltTreeAcquireStoredNs(elem->doc,
				    alias->targetNs->href,
				    alias->targetNs->prefix);

				if (ns == NULL) {
				    xsltTransformError(NULL, cctxt->style, elem,
					"Internal error in "
					"xsltLREBuildEffectiveNs(): "
					"failed to acquire a stored "
					"ns-declaration.\n");
				    cctxt->style->errors++;
				    return(-1);

				}
			    }
			    elem->ns = ns;
			}
		    } else {
		     
			elem->ns = NULL;
		    }
		    break;
		}
		alias = alias->next;
	    }

	    attr = attr->next;
	}
    }
    return(0);
}
