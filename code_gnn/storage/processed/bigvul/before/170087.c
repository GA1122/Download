xsltParseAnyXSLTElem(xsltCompilerCtxtPtr cctxt, xmlNodePtr elem)
{
    if ((cctxt == NULL) || (elem == NULL) ||
	(elem->type != XML_ELEMENT_NODE))
	return(-1);

    elem->psvi = NULL;

    if (! (IS_XSLT_ELEM_FAST(elem)))
	return(-1);
     
    if (cctxt->inode->category == XSLT_ELEMENT_CATEGORY_EXTENSION) {
	cctxt->inode->extContentHandled = 1;
    }

    xsltCompilerNodePush(cctxt, elem);
     
    if (cctxt->inode->prev->curChildType != 0)
	cctxt->inode->type = cctxt->inode->prev->curChildType;
    else
	cctxt->inode->type = xsltGetXSLTElementTypeByNode(cctxt, elem);
     
    if (elem->nsDef != NULL)
	cctxt->inode->inScopeNs =
	    xsltCompilerBuildInScopeNsList(cctxt, elem);
     
    xsltStylePreCompute(cctxt->style, elem);
     

     
    switch (cctxt->inode->type) {
	case XSLT_FUNC_APPLYIMPORTS:
	     
	    goto empty_content;
	case XSLT_FUNC_APPLYTEMPLATES:
	     
	    goto apply_templates;
	case XSLT_FUNC_ATTRIBUTE:
	     
	    goto sequence_constructor;
	case XSLT_FUNC_CALLTEMPLATE:
	     
	    goto call_template;
	case XSLT_FUNC_CHOOSE:
	     
	    goto choose;
	case XSLT_FUNC_COMMENT:
	     
	    goto sequence_constructor;
	case XSLT_FUNC_COPY:
	     
	    goto sequence_constructor;
	case XSLT_FUNC_COPYOF:
	     
	    goto empty_content;
	case XSLT_FUNC_DOCUMENT:  
	     
	    goto sequence_constructor;
	case XSLT_FUNC_ELEMENT:
	     
	    goto sequence_constructor;
	case XSLT_FUNC_FALLBACK:
	     
	    goto sequence_constructor;
	case XSLT_FUNC_FOREACH:
	     
	    goto for_each;
	case XSLT_FUNC_IF:
	     
	    goto sequence_constructor;
	case XSLT_FUNC_OTHERWISE:
	     
	    goto sequence_constructor;
	case XSLT_FUNC_MESSAGE:
	     
	    goto sequence_constructor;
	case XSLT_FUNC_NUMBER:
	     
	    goto empty_content;
	case XSLT_FUNC_PARAM:
	     
	    if ((elem->psvi != NULL) && (cctxt->ivar != NULL)) {
		xsltVarInfoPtr ivar = cctxt->ivar;

		do {
		    if ((ivar->name ==
			 ((xsltStyleItemParamPtr) elem->psvi)->name) &&
			(ivar->nsName ==
			 ((xsltStyleItemParamPtr) elem->psvi)->ns))
		    {
			elem->psvi = NULL;
			xsltTransformError(NULL, cctxt->style, elem,
			    "Redefinition of variable or parameter '%s'.\n",
			    ivar->name);
			cctxt->style->errors++;
			goto error;
		    }
		    ivar = ivar->prev;
		} while (ivar != NULL);
	    }
	     
	    goto sequence_constructor;
	case XSLT_FUNC_PI:
	     
	    goto sequence_constructor;
	case XSLT_FUNC_SORT:
	     
	    goto empty_content;
	case XSLT_FUNC_TEXT:
	     
	    goto text;
	case XSLT_FUNC_VALUEOF:
	     
	    goto empty_content;
	case XSLT_FUNC_VARIABLE:
	     
	    if ((elem->psvi != NULL) && (cctxt->ivar != NULL)) {
		xsltVarInfoPtr ivar = cctxt->ivar;

		do {
		    if ((ivar->name ==
			 ((xsltStyleItemVariablePtr) elem->psvi)->name) &&
			(ivar->nsName ==
			 ((xsltStyleItemVariablePtr) elem->psvi)->ns))
		    {
			elem->psvi = NULL;
			xsltTransformError(NULL, cctxt->style, elem,
			    "Redefinition of variable or parameter '%s'.\n",
			    ivar->name);
			cctxt->style->errors++;
			goto error;
		    }
		    ivar = ivar->prev;
		} while (ivar != NULL);
	    }
	     
	    goto sequence_constructor;
	case XSLT_FUNC_WHEN:
	     
	    goto sequence_constructor;
	case XSLT_FUNC_WITHPARAM:
	     
	    goto sequence_constructor;
	default:
#ifdef WITH_XSLT_DEBUG_PARSING
	    xsltGenericDebug(xsltGenericDebugContext,
		"xsltParseXSLTNode: Unhandled XSLT element '%s'.\n",
		elem->name);
#endif
	    xsltTransformError(NULL, cctxt->style, elem,
		"xsltParseXSLTNode: Internal error; "
		"unhandled XSLT element '%s'.\n", elem->name);
	    cctxt->style->errors++;
	    goto internal_err;
    }

apply_templates:
     
    if (elem->children != NULL) {
	xmlNodePtr child = elem->children;
	do {
	    if (child->type == XML_ELEMENT_NODE) {
		if (IS_XSLT_ELEM_FAST(child)) {
		    if (xmlStrEqual(child->name, BAD_CAST "with-param")) {
			cctxt->inode->curChildType = XSLT_FUNC_WITHPARAM;
			xsltParseAnyXSLTElem(cctxt, child);
		    } else if (xmlStrEqual(child->name, BAD_CAST "sort")) {
			cctxt->inode->curChildType = XSLT_FUNC_SORT;
			xsltParseAnyXSLTElem(cctxt, child);
		    } else
			xsltParseContentError(cctxt->style, child);
		} else
		    xsltParseContentError(cctxt->style, child);
	    }
	    child = child->next;
	} while (child != NULL);
    }
    goto exit;

call_template:
     
    if (elem->children != NULL) {
	xmlNodePtr child = elem->children;
	do {
	    if (child->type == XML_ELEMENT_NODE) {
		if (IS_XSLT_ELEM_FAST(child)) {
		    xsltStyleType type;

		    type = xsltGetXSLTElementTypeByNode(cctxt, child);
		    if (type == XSLT_FUNC_WITHPARAM) {
			cctxt->inode->curChildType = XSLT_FUNC_WITHPARAM;
			xsltParseAnyXSLTElem(cctxt, child);
		    } else {
			xsltParseContentError(cctxt->style, child);
		    }
		} else
		    xsltParseContentError(cctxt->style, child);
	    }
	    child = child->next;
	} while (child != NULL);
    }
    goto exit;

text:
    if (elem->children != NULL) {
	xmlNodePtr child = elem->children;
	do {
	    if ((child->type != XML_TEXT_NODE) &&
		(child->type != XML_CDATA_SECTION_NODE))
	    {
		xsltTransformError(NULL, cctxt->style, elem,
		    "The XSLT 'text' element must have only character "
		    "data as content.\n");
	    }
	    child = child->next;
	} while (child != NULL);
    }
    goto exit;

empty_content:
    if (elem->children != NULL) {
	xmlNodePtr child = elem->children;
	 
	do {
	    if (((child->type != XML_TEXT_NODE) &&
		 (child->type != XML_CDATA_SECTION_NODE)) ||
		(! IS_BLANK_NODE(child)))
	    {
		xsltTransformError(NULL, cctxt->style, elem,
		    "This XSLT element must have no content.\n");
		cctxt->style->errors++;
		break;
	    }
	    child = child->next;
	} while (child != NULL);
    }
    goto exit;

choose:
     
     
    if (elem->children != NULL) {
	xmlNodePtr child = elem->children;
	int nbWhen = 0, nbOtherwise = 0, err = 0;
	do {
	    if (child->type == XML_ELEMENT_NODE) {
		if (IS_XSLT_ELEM_FAST(child)) {
		    xsltStyleType type;

		    type = xsltGetXSLTElementTypeByNode(cctxt, child);
		    if (type == XSLT_FUNC_WHEN) {
			nbWhen++;
			if (nbOtherwise) {
			    xsltParseContentError(cctxt->style, child);
			    err = 1;
			    break;
			}
			cctxt->inode->curChildType = XSLT_FUNC_WHEN;
			xsltParseAnyXSLTElem(cctxt, child);
		    } else if (type == XSLT_FUNC_OTHERWISE) {
			if (! nbWhen) {
			    xsltParseContentError(cctxt->style, child);
			    err = 1;
			    break;
			}
			if (nbOtherwise) {
			    xsltTransformError(NULL, cctxt->style, elem,
				"The XSLT 'choose' element must not contain "
				"more than one XSLT 'otherwise' element.\n");
			    cctxt->style->errors++;
			    err = 1;
			    break;
			}
			nbOtherwise++;
			cctxt->inode->curChildType = XSLT_FUNC_OTHERWISE;
			xsltParseAnyXSLTElem(cctxt, child);
		    } else
			xsltParseContentError(cctxt->style, child);
		} else
		    xsltParseContentError(cctxt->style, child);
	    }
	     
	    child = child->next;
	} while (child != NULL);
	if ((! err) && (! nbWhen)) {
	    xsltTransformError(NULL, cctxt->style, elem,
		"The XSLT element 'choose' must contain at least one "
		"XSLT element 'when'.\n");
		cctxt->style->errors++;
	}
    }
    goto exit;

for_each:
     
     
    if (elem->children != NULL) {
	xmlNodePtr child = elem->children;
	 
	do {
	    if ((child->type == XML_ELEMENT_NODE) &&
		IS_XSLT_ELEM_FAST(child))
	    {
		if (xsltGetXSLTElementTypeByNode(cctxt, child) ==
		    XSLT_FUNC_SORT)
		{
		    cctxt->inode->curChildType = XSLT_FUNC_SORT;
		    xsltParseAnyXSLTElem(cctxt, child);
		} else
		    break;
	    } else
		break;
	    child = child->next;
	} while (child != NULL);
	 
	if (child != NULL)
	    xsltParseSequenceConstructor(cctxt, child);
    }
    goto exit;

sequence_constructor:
     
    if (elem->children != NULL)
	xsltParseSequenceConstructor(cctxt, elem->children);

     
    if ((elem->next != NULL) &&
	((cctxt->inode->type == XSLT_FUNC_VARIABLE) ||
	 (cctxt->inode->type == XSLT_FUNC_PARAM)))
    {
	if ((elem->psvi != NULL) &&
	    (((xsltStyleBasicItemVariablePtr) elem->psvi)->name))
	{
	    xsltCompilerVarInfoPush(cctxt, elem,
		((xsltStyleBasicItemVariablePtr) elem->psvi)->name,
		((xsltStyleBasicItemVariablePtr) elem->psvi)->ns);
	}
    }

error:
exit:
    xsltCompilerNodePop(cctxt, elem);
    return(0);

internal_err:
    xsltCompilerNodePop(cctxt, elem);
    return(-1);
}
