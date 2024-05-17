xsltApplyXSLTTemplate(xsltTransformContextPtr ctxt,
		      xmlNodePtr contextNode,
		      xmlNodePtr list,
		      xsltTemplatePtr templ,
		      xsltStackElemPtr withParams)
{
    int oldVarsBase = 0;
    long start = 0;
    xmlNodePtr cur;
    xsltStackElemPtr tmpParam = NULL;
    xmlDocPtr oldUserFragmentTop, oldLocalFragmentTop;

#ifdef XSLT_REFACTORED
    xsltStyleItemParamPtr iparam;
#else
    xsltStylePreCompPtr iparam;
#endif

#ifdef WITH_DEBUGGER
    int addCallResult = 0;
#endif

    if (ctxt == NULL)
	return;
    if (templ == NULL) {
	xsltTransformError(ctxt, NULL, list,
	    "xsltApplyXSLTTemplate: Bad arguments; @templ is mandatory.\n");
	return;
    }

#ifdef WITH_DEBUGGER
    if (ctxt->debugStatus != XSLT_DEBUG_NONE) {
	if (xsltDebuggerStartSequenceConstructor(ctxt, contextNode,
		list, templ, &addCallResult) == NULL)
	    return;
    }
#endif

    if (list == NULL)
        return;
    CHECK_STOPPED;

     
    if (ctxt->templNr >= ctxt->maxTemplateDepth)
    {
        xsltTransformError(ctxt, NULL, list,
	    "xsltApplyXSLTTemplate: A potential infinite template recursion "
	    "was detected.\n"
	    "You can adjust xsltMaxDepth (--maxdepth) in order to "
	    "raise the maximum number of nested template calls and "
	    "variables/params (currently set to %d).\n",
	    ctxt->maxTemplateDepth);
        xsltDebug(ctxt, contextNode, list, NULL);
        return;
    }

    if (ctxt->varsNr >= ctxt->maxTemplateVars)
	{
        xsltTransformError(ctxt, NULL, list,
	    "xsltApplyXSLTTemplate: A potential infinite template recursion "
	    "was detected.\n"
	    "You can adjust maxTemplateVars (--maxvars) in order to "
	    "raise the maximum number of variables/params (currently set to %d).\n",
	    ctxt->maxTemplateVars);
        xsltDebug(ctxt, contextNode, list, NULL);
        return;
	}

    oldUserFragmentTop = ctxt->tmpRVT;
    ctxt->tmpRVT = NULL;
    oldLocalFragmentTop = ctxt->localRVT;

     
    oldVarsBase = ctxt->varsBase;
    ctxt->varsBase = ctxt->varsNr;

    ctxt->node = contextNode;
    if (ctxt->profile) {
	templ->nbCalls++;
	start = xsltTimestamp();
	profPush(ctxt, 0);
	profCallgraphAdd(templ, ctxt->templ);
    }
     
    templPush(ctxt, templ);

#ifdef WITH_XSLT_DEBUG_PROCESS
    if (templ->name != NULL)
	XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATE,xsltGenericDebug(xsltGenericDebugContext,
	"applying xsl:template '%s'\n", templ->name));
#endif
     
    cur = list;
    do {
	if (cur->type == XML_TEXT_NODE) {
	    cur = cur->next;
	    continue;
	}
	if ((cur->type != XML_ELEMENT_NODE) ||
	    (cur->name[0] != 'p') ||
	    (cur->psvi == NULL) ||
	    (! xmlStrEqual(cur->name, BAD_CAST "param")) ||
	    (! IS_XSLT_ELEM(cur)))
	{
	    break;
	}

	list = cur->next;

#ifdef XSLT_REFACTORED
	iparam = (xsltStyleItemParamPtr) cur->psvi;
#else
	iparam = (xsltStylePreCompPtr) cur->psvi;
#endif

	 
	tmpParam = NULL;
	if (withParams) {
	    tmpParam = withParams;
	    do {
		if ((tmpParam->name == (iparam->name)) &&
		    (tmpParam->nameURI == (iparam->ns)))
		{
		     
		    xsltLocalVariablePush(ctxt, tmpParam, -1);
		    break;
		}
		tmpParam = tmpParam->next;
	    } while (tmpParam != NULL);
	}
	 
	if (tmpParam == NULL) {
	     
	    xsltParseStylesheetParam(ctxt, cur);
	}
	cur = cur->next;
    } while (cur != NULL);
     
    xsltApplySequenceConstructor(ctxt, contextNode, list, templ);

     
    if (ctxt->varsNr > ctxt->varsBase)
	xsltTemplateParamsCleanup(ctxt);
    ctxt->varsBase = oldVarsBase;

     
    if (oldLocalFragmentTop != ctxt->localRVT) {
	xmlDocPtr curdoc = ctxt->localRVT, tmp;

	do {
	    tmp = curdoc;
	    curdoc = (xmlDocPtr) curdoc->next;
	     
	    if (tmp == ctxt->localRVTBase)
	        ctxt->localRVTBase = curdoc;
	    if (tmp->prev)
		tmp->prev->next = (xmlNodePtr) curdoc;
	    if (curdoc)
		curdoc->prev = tmp->prev;
	    xsltReleaseRVT(ctxt, tmp);
	} while (curdoc != oldLocalFragmentTop);
    }
    ctxt->localRVT = oldLocalFragmentTop;

     
    if (ctxt->tmpRVT) {
	xmlDocPtr curdoc = ctxt->tmpRVT, tmp;

	while (curdoc != NULL) {
	    tmp = curdoc;
	    curdoc = (xmlDocPtr) curdoc->next;
	    xsltReleaseRVT(ctxt, tmp);
	}
    }
    ctxt->tmpRVT = oldUserFragmentTop;

     
    templPop(ctxt);
    if (ctxt->profile) {
	long spent, child, total, end;

	end = xsltTimestamp();
	child = profPop(ctxt);
	total = end - start;
	spent = total - child;
	if (spent <= 0) {
	     
	    xsltCalibrateAdjust(spent);
	    spent = 0;
	}

	templ->time += spent;
	if (ctxt->profNr > 0)
	    ctxt->profTab[ctxt->profNr - 1] += total;
    }

#ifdef WITH_DEBUGGER
    if ((ctxt->debugStatus != XSLT_DEBUG_NONE) && (addCallResult)) {
        xslDropCall();
    }
#endif
}
