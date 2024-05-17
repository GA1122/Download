xsltStylePreCompute(xsltStylesheetPtr style, xmlNodePtr node) {
     
    if (style == NULL) {
	if ((node != NULL) && (node->type == XML_ELEMENT_NODE))
	    node->psvi = NULL;
	return;
    }
    if (node == NULL)
	return;
    if (! IS_XSLT_ELEM_FAST(node))
	return;

    node->psvi = NULL;
    if (XSLT_CCTXT(style)->inode->type != 0) {
	switch (XSLT_CCTXT(style)->inode->type) {
	    case XSLT_FUNC_APPLYTEMPLATES:
		xsltApplyTemplatesComp(style, node);
		break;
	    case XSLT_FUNC_WITHPARAM:
		xsltWithParamComp(style, node);
		break;
	    case XSLT_FUNC_VALUEOF:
		xsltValueOfComp(style, node);
		break;
	    case XSLT_FUNC_COPY:
		xsltCopyComp(style, node);
		break;
	    case XSLT_FUNC_COPYOF:
		xsltCopyOfComp(style, node);
		break;
	    case XSLT_FUNC_IF:
		xsltIfComp(style, node);
		break;
	    case XSLT_FUNC_CHOOSE:
		xsltChooseComp(style, node);
		break;
	    case XSLT_FUNC_WHEN:
		xsltWhenComp(style, node);
		break;
	    case XSLT_FUNC_OTHERWISE:
		 
		return;
	    case XSLT_FUNC_FOREACH:
		xsltForEachComp(style, node);
		break;
	    case XSLT_FUNC_APPLYIMPORTS:
		xsltApplyImportsComp(style, node);
		break;
	    case XSLT_FUNC_ATTRIBUTE:
		xsltAttributeComp(style, node);
		break;
	    case XSLT_FUNC_ELEMENT:
		xsltElementComp(style, node);
		break;
	    case XSLT_FUNC_SORT:
		xsltSortComp(style, node);
		break;
	    case XSLT_FUNC_COMMENT:
		xsltCommentComp(style, node);
		break;
	    case XSLT_FUNC_NUMBER:
		xsltNumberComp(style, node);
		break;
	    case XSLT_FUNC_PI:
		xsltProcessingInstructionComp(style, node);
		break;
	    case XSLT_FUNC_CALLTEMPLATE:
		xsltCallTemplateComp(style, node);
		break;
	    case XSLT_FUNC_PARAM:
		xsltParamComp(style, node);
		break;
	    case XSLT_FUNC_VARIABLE:
		xsltVariableComp(style, node);
		break;
	    case XSLT_FUNC_FALLBACK:
		 
		return;
	    case XSLT_FUNC_DOCUMENT:
		 
		node->psvi = (void *) xsltDocumentComp(style, node,
		    (xsltTransformFunction) xsltDocumentElem);
		break;
	    case XSLT_FUNC_MESSAGE:
		 
		return;
	    default:
		 
		xsltTransformError(NULL, style, node,
		    "Internal error: (xsltStylePreCompute) cannot handle "
		    "the XSLT element '%s'.\n", node->name);
		style->errors++;
		return;
	}
    } else {
	 
	if (IS_XSLT_NAME(node, "apply-templates")) {
	    xsltApplyTemplatesComp(style, node);
	} else if (IS_XSLT_NAME(node, "with-param")) {
	    xsltWithParamComp(style, node);
	} else if (IS_XSLT_NAME(node, "value-of")) {
	    xsltValueOfComp(style, node);
	} else if (IS_XSLT_NAME(node, "copy")) {
	    xsltCopyComp(style, node);
	} else if (IS_XSLT_NAME(node, "copy-of")) {
	    xsltCopyOfComp(style, node);
	} else if (IS_XSLT_NAME(node, "if")) {
	    xsltIfComp(style, node);
	} else if (IS_XSLT_NAME(node, "choose")) {
	    xsltChooseComp(style, node);
	} else if (IS_XSLT_NAME(node, "when")) {
	    xsltWhenComp(style, node);
	} else if (IS_XSLT_NAME(node, "otherwise")) {
	     
	    return;
	} else if (IS_XSLT_NAME(node, "for-each")) {
	    xsltForEachComp(style, node);
	} else if (IS_XSLT_NAME(node, "apply-imports")) {
	    xsltApplyImportsComp(style, node);
	} else if (IS_XSLT_NAME(node, "attribute")) {
	    xsltAttributeComp(style, node);
	} else if (IS_XSLT_NAME(node, "element")) {
	    xsltElementComp(style, node);
	} else if (IS_XSLT_NAME(node, "sort")) {
	    xsltSortComp(style, node);
	} else if (IS_XSLT_NAME(node, "comment")) {
	    xsltCommentComp(style, node);
	} else if (IS_XSLT_NAME(node, "number")) {
	    xsltNumberComp(style, node);
	} else if (IS_XSLT_NAME(node, "processing-instruction")) {
	    xsltProcessingInstructionComp(style, node);
	} else if (IS_XSLT_NAME(node, "call-template")) {
	    xsltCallTemplateComp(style, node);
	} else if (IS_XSLT_NAME(node, "param")) {
	    xsltParamComp(style, node);
	} else if (IS_XSLT_NAME(node, "variable")) {
	    xsltVariableComp(style, node);
	} else if (IS_XSLT_NAME(node, "fallback")) {
	     
	    return;
	} else if (IS_XSLT_NAME(node, "document")) {
	     
	    node->psvi = (void *) xsltDocumentComp(style, node,
		(xsltTransformFunction) xsltDocumentElem);
	} else if (IS_XSLT_NAME(node, "output")) {
	     
	    return;
	} else if (IS_XSLT_NAME(node, "preserve-space")) {
	     
	    return;
	} else if (IS_XSLT_NAME(node, "strip-space")) {
	     
	    return;
	} else if (IS_XSLT_NAME(node, "key")) {
	     
	    return;
	} else if (IS_XSLT_NAME(node, "message")) {
	    return;
	} else if (IS_XSLT_NAME(node, "attribute-set")) {
	     
	    return;
	} else if (IS_XSLT_NAME(node, "namespace-alias")) {
	     
	    return;
	} else if (IS_XSLT_NAME(node, "decimal-format")) {
	     
	    return;
	} else if (IS_XSLT_NAME(node, "include")) {
	     
	} else {
	     
	    xsltTransformError(NULL, style, node,
		"Internal error: (xsltStylePreCompute) cannot handle "
		"the XSLT element '%s'.\n", node->name);
		style->errors++;
	    return;
	}
    }
     
    if (node->psvi != NULL) {
	((xsltStylePreCompPtr) node->psvi)->inScopeNs =
	    XSLT_CCTXT(style)->inode->inScopeNs;
    }
}
