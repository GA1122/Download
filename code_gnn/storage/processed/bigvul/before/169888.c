xsltStyleInitializeStylesheetModule(xsltStylesheetPtr style,
				     const xmlChar * URI)
{
    xsltExtDataPtr dataContainer;
    void *userData = NULL;
    xsltExtModulePtr module;

    if ((style == NULL) || (URI == NULL))
	return(NULL);

    if (xsltExtensionsHash == NULL) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
	    "Not registered extension module: %s\n", URI);
#endif
	return(NULL);
    }

    xmlMutexLock(xsltExtMutex);

    module = xmlHashLookup(xsltExtensionsHash, URI);

    xmlMutexUnlock(xsltExtMutex);

    if (module == NULL) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
	    "Not registered extension module: %s\n", URI);
#endif
	return (NULL);
    }
     
    if (style->extInfos == NULL) {
	style->extInfos = xmlHashCreate(10);
	if (style->extInfos == NULL)
	    return (NULL);
    }
     
    if (module->styleInitFunc == NULL) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
	    "Initializing module with *no* callback: %s\n", URI);
#endif
    } else {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
	    "Initializing module with callback: %s\n", URI);
#endif
	 
	userData = module->styleInitFunc(style, URI);
    }
     
    dataContainer = xsltNewExtData(module, userData);
    if (dataContainer == NULL)
	return (NULL);

    if (xmlHashAddEntry(style->extInfos, URI,
	(void *) dataContainer) < 0)
    {
	xsltTransformError(NULL, style, NULL,
	    "Failed to register module '%s'.\n", URI);
	style->errors++;
	if (module->styleShutdownFunc)
	    module->styleShutdownFunc(style, URI, userData);
	xsltFreeExtData(dataContainer);
	return (NULL);
    }

    return(dataContainer);
}
