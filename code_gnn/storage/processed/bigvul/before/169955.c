xsltDocumentComp(xsltStylesheetPtr style, xmlNodePtr inst,
		 xsltTransformFunction function ATTRIBUTE_UNUSED) {
#ifdef XSLT_REFACTORED
    xsltStyleItemDocumentPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif
    const xmlChar *filename = NULL;

     
#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemDocumentPtr)
	xsltNewStylePreComp(style, XSLT_FUNC_DOCUMENT);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_DOCUMENT);
#endif

    if (comp == NULL)
	return (NULL);
    comp->inst = inst;
    comp->ver11 = 0;

    if (xmlStrEqual(inst->name, (const xmlChar *) "output")) {
#ifdef WITH_XSLT_DEBUG_EXTRA
	xsltGenericDebug(xsltGenericDebugContext,
	    "Found saxon:output extension\n");
#endif
	 
	filename = xsltEvalStaticAttrValueTemplate(style, inst,
			 (const xmlChar *)"file",
			 NULL, &comp->has_filename);
    } else if (xmlStrEqual(inst->name, (const xmlChar *) "write")) {
#ifdef WITH_XSLT_DEBUG_EXTRA
	xsltGenericDebug(xsltGenericDebugContext,
	    "Found xalan:write extension\n");
#endif
	 
	 
    } else if (xmlStrEqual(inst->name, (const xmlChar *) "document")) {
	if (inst->ns != NULL) {
	    if (xmlStrEqual(inst->ns->href, XSLT_NAMESPACE)) {
		 
		comp->ver11 = 1;
#ifdef WITH_XSLT_DEBUG_EXTRA
		xsltGenericDebug(xsltGenericDebugContext,
		    "Found xslt11:document construct\n");
#endif
	    } else {
		if (xmlStrEqual(inst->ns->href,
		    (const xmlChar *)"http://exslt.org/common")) {
		     
#ifdef WITH_XSLT_DEBUG_EXTRA
		    xsltGenericDebug(xsltGenericDebugContext,
			"Found exslt:document extension\n");
#endif
		} else if (xmlStrEqual(inst->ns->href, XSLT_XT_NAMESPACE)) {
		     
#ifdef WITH_XSLT_DEBUG_EXTRA
		    xsltGenericDebug(xsltGenericDebugContext,
			"Found xt:document extension\n");
#endif
		}
	    }
	}
	 
	filename = xsltEvalStaticAttrValueTemplate(style, inst,
	    (const xmlChar *)"href", NULL, &comp->has_filename);
    }
    if (!comp->has_filename) {
	goto error;
    }
    comp->filename = filename;

error:
    return ((xsltElemPreCompPtr) comp);
}
