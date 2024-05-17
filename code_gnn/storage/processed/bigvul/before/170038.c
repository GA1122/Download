xsltParseStylesheetVariable(xsltTransformContextPtr ctxt, xmlNodePtr inst)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemVariablePtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((inst == NULL) || (ctxt == NULL) || (inst->type != XML_ELEMENT_NODE))
	return;

    comp = inst->psvi;
    if (comp == NULL) {
        xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltParseStylesheetVariable(): "
	    "The XSLT 'variable' instruction was not compiled.\n");
        return;
    }
    if (comp->name == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltParseStylesheetVariable(): "
	    "The attribute 'name' was not compiled.\n");
	return;
    }

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	"Registering variable '%s'\n", comp->name));
#endif

    xsltRegisterVariable(ctxt, (xsltStylePreCompPtr) comp, inst->children, 0);
}
