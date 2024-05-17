xsltNewTransformContext(xsltStylesheetPtr style, xmlDocPtr doc) {
    xsltTransformContextPtr cur;
    xsltDocumentPtr docu;
    int i;

    xsltInitGlobals();

    cur = (xsltTransformContextPtr) xmlMalloc(sizeof(xsltTransformContext));
    if (cur == NULL) {
	xsltTransformError(NULL, NULL, (xmlNodePtr)doc,
		"xsltNewTransformContext : malloc failed\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xsltTransformContext));

    cur->cache = xsltTransformCacheCreate();
    if (cur->cache == NULL)
	goto internal_err;
     
    cur->dict = xmlDictCreateSub(style->dict);
    cur->internalized = ((style->internalized) && (cur->dict != NULL));
#ifdef WITH_XSLT_DEBUG
    xsltGenericDebug(xsltGenericDebugContext,
	     "Creating sub-dictionary from stylesheet for transformation\n");
#endif

     
    cur->templTab = (xsltTemplatePtr *)
	        xmlMalloc(10 * sizeof(xsltTemplatePtr));
    if (cur->templTab == NULL) {
	xsltTransformError(NULL, NULL, (xmlNodePtr) doc,
		"xsltNewTransformContext: out of memory\n");
	goto internal_err;
    }
    cur->templNr = 0;
    cur->templMax = 5;
    cur->templ = NULL;
    cur->maxTemplateDepth = xsltMaxDepth;

     
    cur->varsTab = (xsltStackElemPtr *)
	        xmlMalloc(10 * sizeof(xsltStackElemPtr));
    if (cur->varsTab == NULL) {
        xmlGenericError(xmlGenericErrorContext,
		"xsltNewTransformContext: out of memory\n");
	goto internal_err;
    }
    cur->varsNr = 0;
    cur->varsMax = 10;
    cur->vars = NULL;
    cur->varsBase = 0;
    cur->maxTemplateVars = xsltMaxVars;

     
    cur->profTab = NULL;
    cur->profNr = 0;
    cur->profMax = 0;
    cur->prof = 0;

    cur->style = style;
    xmlXPathInit();
    cur->xpathCtxt = xmlXPathNewContext(doc);
    if (cur->xpathCtxt == NULL) {
	xsltTransformError(NULL, NULL, (xmlNodePtr) doc,
		"xsltNewTransformContext : xmlXPathNewContext failed\n");
	goto internal_err;
    }
     
    if (xmlXPathContextSetCache(cur->xpathCtxt, 1, -1, 0) == -1)
	goto internal_err;
     
    if (style->extrasNr != 0) {
	cur->extrasMax = style->extrasNr + 20;
	cur->extras = (xsltRuntimeExtraPtr)
	    xmlMalloc(cur->extrasMax * sizeof(xsltRuntimeExtra));
	if (cur->extras == NULL) {
	    xmlGenericError(xmlGenericErrorContext,
		    "xsltNewTransformContext: out of memory\n");
	    goto internal_err;
	}
	cur->extrasNr = style->extrasNr;
	for (i = 0;i < cur->extrasMax;i++) {
	    cur->extras[i].info = NULL;
	    cur->extras[i].deallocate = NULL;
	    cur->extras[i].val.ptr = NULL;
	}
    } else {
	cur->extras = NULL;
	cur->extrasNr = 0;
	cur->extrasMax = 0;
    }

    XSLT_REGISTER_VARIABLE_LOOKUP(cur);
    XSLT_REGISTER_FUNCTION_LOOKUP(cur);
    cur->xpathCtxt->nsHash = style->nsHash;
     
    xsltInitCtxtExts(cur);
     
    if (xslDebugStatus == XSLT_DEBUG_NONE)
        xmlXPathOrderDocElems(doc);
     
    cur->parserOptions = XSLT_PARSE_OPTIONS;
    docu = xsltNewDocument(cur, doc);
    if (docu == NULL) {
	xsltTransformError(cur, NULL, (xmlNodePtr)doc,
		"xsltNewTransformContext : xsltNewDocument failed\n");
	goto internal_err;
    }
    docu->main = 1;
    cur->document = docu;
    cur->inst = NULL;
    cur->outputFile = NULL;
    cur->sec = xsltGetDefaultSecurityPrefs();
    cur->debugStatus = xslDebugStatus;
    cur->traceCode = (unsigned long*) &xsltDefaultTrace;
    cur->xinclude = xsltGetXIncludeDefault();
    cur->keyInitLevel = 0;

    return(cur);

internal_err:
    if (cur != NULL)
	xsltFreeTransformContext(cur);
    return(NULL);
}
