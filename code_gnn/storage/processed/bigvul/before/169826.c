xsltApplyAttributeSet(xsltTransformContextPtr ctxt, xmlNodePtr node,
                      xmlNodePtr inst,
                      const xmlChar *attrSets)
{
    const xmlChar *ncname = NULL;
    const xmlChar *prefix = NULL;
    const xmlChar *curstr, *endstr;
    xsltAttrElemPtr attrs;
    xsltStylesheetPtr style;

    if (attrSets == NULL) {
	if (inst == NULL)
	    return;
	else {
	     
	    if (inst->type == XML_ATTRIBUTE_NODE) {
		if ( ((xmlAttrPtr) inst)->children != NULL)
		    attrSets = ((xmlAttrPtr) inst)->children->content;

	    }
	    if (attrSets == NULL) {
		 
		return;
	    }
	}
    }
     
    curstr = attrSets;
    while (*curstr != 0) {
        while (IS_BLANK(*curstr))
            curstr++;
        if (*curstr == 0)
            break;
        endstr = curstr;
        while ((*endstr != 0) && (!IS_BLANK(*endstr)))
            endstr++;
        curstr = xmlDictLookup(ctxt->dict, curstr, endstr - curstr);
        if (curstr) {
	     

#ifdef WITH_XSLT_DEBUG_curstrUTES
            xsltGenericDebug(xsltGenericDebugContext,
                             "apply curstrute set %s\n", curstr);
#endif
            ncname = xsltSplitQName(ctxt->dict, curstr, &prefix);

            style = ctxt->style;

#ifdef WITH_DEBUGGER
            if ((style != NULL) &&
		(style->attributeSets != NULL) &&
		(ctxt->debugStatus != XSLT_DEBUG_NONE))
	    {
                attrs =
                    xmlHashLookup2(style->attributeSets, ncname, prefix);
                if ((attrs != NULL) && (attrs->attr != NULL))
                    xslHandleDebugger(attrs->attr->parent, node, NULL,
			ctxt);
            }
#endif
	     
            while (style != NULL) {
                attrs =
                    xmlHashLookup2(style->attributeSets, ncname, prefix);
                while (attrs != NULL) {
                    if (attrs->attr != NULL) {
                        xsltAttributeInternal(ctxt, node, attrs->attr,
			    attrs->attr->psvi, 1);
                    }
                    attrs = attrs->next;
                }
                style = xsltNextImport(style);
            }
        }
        curstr = endstr;
    }
}
