xsltApplyImports(xsltTransformContextPtr ctxt, xmlNodePtr contextNode,
	         xmlNodePtr inst,
		 xsltStylePreCompPtr comp ATTRIBUTE_UNUSED)
{
    xsltTemplatePtr templ;

    if ((ctxt == NULL) || (inst == NULL))
	return;

    if (comp == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltApplyImports(): "
	    "The XSLT 'apply-imports' instruction was not compiled.\n");
	return;
    }
     
    if (ctxt->currentTemplateRule == NULL) {
	 
	xsltTransformError(ctxt, NULL, inst,
	     "It is an error to call 'apply-imports' "
	     "when there's no current template rule.\n");
	return;
    }
     
    templ = xsltGetTemplate(ctxt, contextNode,
	ctxt->currentTemplateRule->style);

    if (templ != NULL) {
	xsltTemplatePtr oldCurTemplRule = ctxt->currentTemplateRule;
	 
	ctxt->currentTemplateRule = templ;
	 
	xsltApplyXSLTTemplate(ctxt, contextNode, templ->content,
	    templ, NULL);

	ctxt->currentTemplateRule = oldCurTemplRule;
    }
}
