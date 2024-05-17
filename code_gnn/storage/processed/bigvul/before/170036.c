xsltParseStylesheetCallerParam(xsltTransformContextPtr ctxt, xmlNodePtr inst)
{
#ifdef XSLT_REFACTORED
    xsltStyleBasicItemVariablePtr comp;
#else
    xsltStylePreCompPtr comp;
#endif
    xmlNodePtr tree = NULL;  
    xsltStackElemPtr param = NULL;

    if ((ctxt == NULL) || (inst == NULL) || (inst->type != XML_ELEMENT_NODE))
	return(NULL);

#ifdef XSLT_REFACTORED
    comp = (xsltStyleBasicItemVariablePtr) inst->psvi;
#else
    comp = (xsltStylePreCompPtr) inst->psvi;
#endif

    if (comp == NULL) {
        xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltParseStylesheetCallerParam(): "
	    "The XSLT 'with-param' instruction was not compiled.\n");
        return(NULL);
    }
    if (comp->name == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltParseStylesheetCallerParam(): "
	    "XSLT 'with-param': The attribute 'name' was not compiled.\n");
	return(NULL);
    }

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	    "Handling xsl:with-param %s\n", comp->name));
#endif

    if (comp->select == NULL) {
	tree = inst->children;
    } else {
#ifdef WITH_XSLT_DEBUG_VARIABLE
	XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	    "        select %s\n", comp->select));
#endif
	tree = inst;
    }

    param = xsltBuildVariable(ctxt, (xsltStylePreCompPtr) comp, tree);

    return(param);
}
