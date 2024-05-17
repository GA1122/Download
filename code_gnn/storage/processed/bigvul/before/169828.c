xsltAttributeInternal(xsltTransformContextPtr ctxt,
		      xmlNodePtr contextNode,
                      xmlNodePtr inst,
		      xsltStylePreCompPtr castedComp,
                      int fromAttributeSet)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemAttributePtr comp =
	(xsltStyleItemAttributePtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    xmlNodePtr targetElem;
    xmlChar *prop = NULL;
    const xmlChar *name = NULL, *prefix = NULL, *nsName = NULL;
    xmlChar *value = NULL;
    xmlNsPtr ns = NULL;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (contextNode == NULL) || (inst == NULL) ||
        (inst->type != XML_ELEMENT_NODE) )
        return;

     
    if (!comp->has_name)
        return;
     
     

    if (comp == NULL) {
        xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltAttributeInternal(): "
	    "The XSLT 'attribute' instruction was not compiled.\n");
        return;
    }
     
    if (ctxt->insert == NULL)
        return;
     
    targetElem = ctxt->insert;
    if (targetElem->type != XML_ELEMENT_NODE)
	return;

     
    if (targetElem->children != NULL) {
	 
        xsltTransformError(ctxt, NULL, inst,
	    "xsl:attribute: Cannot add attributes to an "
	    "element if children have been already added "
	    "to the element.\n");
        return;
    }

     

#ifdef WITH_DEBUGGER
    if (ctxt->debugStatus != XSLT_DEBUG_NONE)
        xslHandleDebugger(inst, contextNode, NULL, ctxt);
#endif

    if (comp->name == NULL) {
	 
        prop = xsltEvalAttrValueTemplate(ctxt, inst,
	    (const xmlChar *) "name", XSLT_NAMESPACE);
        if (prop == NULL) {
            xsltTransformError(ctxt, NULL, inst,
		"xsl:attribute: The attribute 'name' is missing.\n");
            goto error;
        }
	if (xmlValidateQName(prop, 0)) {
	    xsltTransformError(ctxt, NULL, inst,
		"xsl:attribute: The effective name '%s' is not a "
		"valid QName.\n", prop);
	     
	}

	 
	if (xmlStrEqual(prop, BAD_CAST "xmlns")) {
            xsltTransformError(ctxt, NULL, inst,
                "xsl:attribute: The effective name 'xmlns' is not allowed.\n");
	    xmlFree(prop);
	    goto error;
	}

	name = xsltSplitQName(ctxt->dict, prop, &prefix);
	xmlFree(prop);
    } else {
	 
#ifdef XSLT_REFACTORED
	prefix = comp->nsPrefix;
	name = comp->name;
#else
	name = xsltSplitQName(ctxt->dict, comp->name, &prefix);
#endif
    }

     
    if (comp->has_ns) {
	 
	if (comp->ns != NULL) {
	     
	    if (comp->ns[0] != 0)
		nsName = comp->ns;
	} else {
	    xmlChar *tmpNsName;
	     
	     
	    tmpNsName = xsltEvalAttrValueTemplate(ctxt, inst,
		(const xmlChar *) "namespace", XSLT_NAMESPACE);
	     
	    if ((tmpNsName != NULL) && (tmpNsName[0] != 0))
		nsName = xmlDictLookup(ctxt->dict, BAD_CAST tmpNsName, -1);
	    xmlFree(tmpNsName);
	}

        if (xmlStrEqual(nsName, BAD_CAST "http://www.w3.org/2000/xmlns/")) {
            xsltTransformError(ctxt, NULL, inst,
                "xsl:attribute: Namespace http://www.w3.org/2000/xmlns/ "
                "forbidden.\n");
            goto error;
        }
        if (xmlStrEqual(nsName, XML_XML_NAMESPACE)) {
            prefix = BAD_CAST "xml";
        } else if (xmlStrEqual(prefix, BAD_CAST "xml")) {
            prefix = NULL;
        }
    } else if (prefix != NULL) {
	 
	ns = xmlSearchNs(inst->doc, inst, prefix);
	if (ns == NULL) {
	     
	    xsltTransformError(ctxt, NULL, inst,
		"xsl:attribute: The QName '%s:%s' has no "
		"namespace binding in scope in the stylesheet; "
		"this is an error, since the namespace was not "
		"specified by the instruction itself.\n", prefix, name);
	} else
	    nsName = ns->href;
    }

    if (fromAttributeSet) {
	 
	attr = xmlHasNsProp(targetElem, name, nsName);
	if (attr != NULL)
	    return;
    }

     
    ns = NULL;

#if 0
    if (0) {
	 
	if (nsName != NULL) {
	     
	    ns = xsltTreeAcquireStoredNs(some doc, nsName, prefix);
	}
    }
#endif

    if (nsName != NULL) {
	 
	 
	if ((prefix == NULL) || xmlStrEqual(prefix, BAD_CAST "xmlns")) {
	    xmlChar *pref = xmlStrdup(BAD_CAST "ns_1");

	    ns = xsltGetSpecialNamespace(ctxt, inst, nsName, pref, targetElem);

	    xmlFree(pref);
	} else {
	    ns = xsltGetSpecialNamespace(ctxt, inst, nsName, prefix,
		targetElem);
	}
	if (ns == NULL) {
	    xsltTransformError(ctxt, NULL, inst,
		"Namespace fixup error: Failed to acquire an in-scope "
		"namespace binding for the generated attribute '{%s}%s'.\n",
		nsName, name);
	    goto error;
	}
    }
     
    if (inst->children == NULL) {
	 
	attr = xmlSetNsProp(ctxt->insert, ns, name, (const xmlChar *) "");
    } else if ((inst->children->next == NULL) &&
	    ((inst->children->type == XML_TEXT_NODE) ||
	     (inst->children->type == XML_CDATA_SECTION_NODE)))
    {
	xmlNodePtr copyTxt;

	 
	attr = xmlSetNsProp(ctxt->insert, ns, name, NULL);
	if (attr == NULL)  
	    goto error;
	 
	if (ctxt->internalized &&
	    (ctxt->insert->doc != NULL) &&
	    (ctxt->insert->doc->dict == ctxt->dict))
	{
	    copyTxt = xmlNewText(NULL);
	    if (copyTxt == NULL)  
		goto error;
	     
	    copyTxt->content = inst->children->content;
	     
	    if (inst->children->name == xmlStringTextNoenc)
		copyTxt->name = xmlStringTextNoenc;
	} else {
	     
	    copyTxt = xmlNewText(inst->children->content);
	    if (copyTxt == NULL)  
		goto error;
	}
	attr->children = attr->last = copyTxt;
	copyTxt->parent = (xmlNodePtr) attr;
	copyTxt->doc = attr->doc;
	 
	if (inst->children->name == xmlStringTextNoenc)
	    copyTxt->name = xmlStringTextNoenc;

         
        if ((copyTxt->content != NULL) &&
            (xmlIsID(attr->doc, attr->parent, attr)))
            xmlAddID(NULL, attr->doc, copyTxt->content, attr);
    } else {
	 
	value = xsltEvalTemplateString(ctxt, contextNode, inst);
	if (value != NULL) {
	    attr = xmlSetNsProp(ctxt->insert, ns, name, value);
	    xmlFree(value);
	} else {
	     
	    attr = xmlSetNsProp(ctxt->insert, ns, name,
		(const xmlChar *) "");
	}
    }

error:
    return;
}
