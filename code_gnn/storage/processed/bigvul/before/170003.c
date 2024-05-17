xsltProcessOneNode(xsltTransformContextPtr ctxt, xmlNodePtr contextNode,
	           xsltStackElemPtr withParams)
{
    xsltTemplatePtr templ;
    xmlNodePtr oldNode;

    templ = xsltGetTemplate(ctxt, contextNode, NULL);
     
    if (templ == NULL) {
#ifdef WITH_XSLT_DEBUG_PROCESS
	if (contextNode->type == XML_DOCUMENT_NODE) {
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: no template found for /\n"));
	} else if (contextNode->type == XML_CDATA_SECTION_NODE) {
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: no template found for CDATA\n"));
	} else if (contextNode->type == XML_ATTRIBUTE_NODE) {
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: no template found for attribute %s\n",
	                     ((xmlAttrPtr) contextNode)->name));
	} else  {
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: no template found for %s\n", contextNode->name));
        }
#endif
	oldNode = ctxt->node;
	ctxt->node = contextNode;
	xsltDefaultProcessOneNode(ctxt, contextNode, withParams);
	ctxt->node = oldNode;
	return;
    }

    if (contextNode->type == XML_ATTRIBUTE_NODE) {
	xsltTemplatePtr oldCurTempRule = ctxt->currentTemplateRule;
	 
	ctxt->currentTemplateRule = templ;

#ifdef WITH_XSLT_DEBUG_PROCESS
	XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: applying template '%s' for attribute %s\n",
	                 templ->match, contextNode->name));
#endif
	xsltApplyXSLTTemplate(ctxt, contextNode, templ->content, templ, withParams);

	ctxt->currentTemplateRule = oldCurTempRule;
    } else {
	xsltTemplatePtr oldCurTempRule = ctxt->currentTemplateRule;
	 
	ctxt->currentTemplateRule = templ;

#ifdef WITH_XSLT_DEBUG_PROCESS
	if (contextNode->type == XML_DOCUMENT_NODE) {
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: applying template '%s' for /\n",
	                     templ->match));
	} else {
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: applying template '%s' for %s\n",
	                     templ->match, contextNode->name));
        }
#endif
	xsltApplyXSLTTemplate(ctxt, contextNode, templ->content, templ, withParams);

	ctxt->currentTemplateRule = oldCurTempRule;
    }
}
