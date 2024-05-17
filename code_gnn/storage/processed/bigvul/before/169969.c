xsltWhenComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemWhenPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL) || (inst->type != XML_ELEMENT_NODE))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemWhenPtr)
	xsltNewStylePreComp(style, XSLT_FUNC_WHEN);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_WHEN);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    comp->test = xsltGetCNsProp(style, inst, (const xmlChar *)"test", XSLT_NAMESPACE);
    if (comp->test == NULL) {
	xsltTransformError(NULL, style, inst,
	     "xsl:when : test is not defined\n");
	if (style != NULL) style->errors++;
	return;
    }
    comp->comp = xsltXPathCompile(style, comp->test);
    if (comp->comp == NULL) {
	xsltTransformError(NULL, style, inst,
	     "xsl:when : could not compile test expression '%s'\n",
	                 comp->test);
	if (style != NULL) style->errors++;
    }
}