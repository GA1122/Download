xsltRegisterVariable(xsltTransformContextPtr ctxt,
		     xsltStylePreCompPtr castedComp,
		     xmlNodePtr tree, int isParam)
{
#ifdef XSLT_REFACTORED
    xsltStyleBasicItemVariablePtr comp =
	(xsltStyleBasicItemVariablePtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
    int present;
#endif
    xsltStackElemPtr variable;

#ifdef XSLT_REFACTORED
     
#else
    present = xsltCheckStackElem(ctxt, comp->name, comp->ns);
    if (isParam == 0) {
	if ((present != 0) && (present != 3)) {
	     
	    xsltTransformError(ctxt, NULL, comp->inst,
		"XSLT-variable: Redefinition of variable '%s'.\n", comp->name);
	    return(0);
	}
    } else if (present != 0) {
	if ((present == 1) || (present == 2)) {
	     
	    xsltTransformError(ctxt, NULL, comp->inst,
		"XSLT-param: Redefinition of parameter '%s'.\n", comp->name);
	    return(0);
	}
#ifdef WITH_XSLT_DEBUG_VARIABLE
	XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
		 "param %s defined by caller\n", comp->name));
#endif
	return(0);
    }
#endif  

    variable = xsltBuildVariable(ctxt, (xsltStylePreCompPtr) comp, tree);
    xsltAddStackElem(ctxt, variable);
    return(0);
}
