xsltParseStylesheetParam(xsltTransformContextPtr ctxt, xmlNodePtr cur)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemParamPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((cur == NULL) || (ctxt == NULL) || (cur->type != XML_ELEMENT_NODE))
	return;

    comp = cur->psvi;
    if ((comp == NULL) || (comp->name == NULL)) {
	xsltTransformError(ctxt, NULL, cur,
	    "Internal error in xsltParseStylesheetParam(): "
	    "The XSLT 'param' declaration was not compiled correctly.\n");
	return;
    }

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	"Registering param %s\n", comp->name));
#endif

    xsltRegisterVariable(ctxt, (xsltStylePreCompPtr) comp, cur->children, 1);
}
