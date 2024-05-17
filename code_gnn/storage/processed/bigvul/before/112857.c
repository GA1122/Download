xsltEvalAttrValueTemplate(xsltTransformContextPtr ctxt, xmlNodePtr inst,
	                  const xmlChar *name, const xmlChar *ns)
{
    xmlChar *ret;
    xmlChar *expr;

    if ((ctxt == NULL) || (inst == NULL) || (name == NULL))
	return(NULL);

    expr = xsltGetNsProp(inst, name, ns);
    if (expr == NULL)
	return(NULL);

     

    ret = xsltAttrTemplateValueProcessNode(ctxt, expr, inst);
#ifdef WITH_XSLT_DEBUG_TEMPLATES
    XSLT_TRACE(ctxt,XSLT_TRACE_TEMPLATES,xsltGenericDebug(xsltGenericDebugContext,
	 "xsltEvalAttrValueTemplate: %s returns %s\n", expr, ret));
#endif
    if (expr != NULL)
	xmlFree(expr);
    return(ret);
}
