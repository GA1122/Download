xsltParseGlobalVariable(xsltStylesheetPtr style, xmlNodePtr cur)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemVariablePtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((cur == NULL) || (style == NULL) || (cur->type != XML_ELEMENT_NODE))
	return;

#ifdef XSLT_REFACTORED
     
    comp = (xsltStyleItemVariablePtr) cur->psvi;
#else
    xsltStylePreCompute(style, cur);
    comp = (xsltStylePreCompPtr) cur->psvi;
#endif
    if (comp == NULL) {
	xsltTransformError(NULL, style, cur,
	     "xsl:variable : compilation failed\n");
	return;
    }

    if (comp->name == NULL) {
	xsltTransformError(NULL, style, cur,
	    "xsl:variable : missing name attribute\n");
	return;
    }

     
    if (cur->children != NULL) {
#ifdef XSLT_REFACTORED
        xsltParseSequenceConstructor(XSLT_CCTXT(style), cur->children);
#else
        xsltParseTemplateContent(style, cur);
#endif
    }
#ifdef WITH_XSLT_DEBUG_VARIABLE
    xsltGenericDebug(xsltGenericDebugContext,
	"Registering global variable %s\n", comp->name);
#endif

    xsltRegisterGlobalVariable(style, comp->name, comp->ns,
	comp->select, cur->children, (xsltStylePreCompPtr) comp,
	NULL);
}
