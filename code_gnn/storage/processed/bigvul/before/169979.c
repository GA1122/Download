xsltApplyOneTemplate(xsltTransformContextPtr ctxt,
		     xmlNodePtr contextNode,
                     xmlNodePtr list,
		     xsltTemplatePtr templ ATTRIBUTE_UNUSED,
                     xsltStackElemPtr params)
{
    if ((ctxt == NULL) || (list == NULL))
	return;
    CHECK_STOPPED;

    if (params) {
	 
	int oldVarsNr = ctxt->varsNr;

	 
	while (params != NULL) {
	    xsltLocalVariablePush(ctxt, params, -1);
	    params = params->next;
	}
	xsltApplySequenceConstructor(ctxt, contextNode, list, templ);
	 
	xsltLocalVariablePop(ctxt, oldVarsNr, -2);
    } else
	xsltApplySequenceConstructor(ctxt, contextNode, list, templ);
}
