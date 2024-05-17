xmlInitializeGlobalState(xmlGlobalStatePtr gs)
{
#ifdef DEBUG_GLOBALS
    fprintf(stderr, "Initializing globals at %lu for thread %d\n",
	    (unsigned long) gs, xmlGetThreadId());
#endif

     
    if (xmlThrDefMutex == NULL)
        xmlInitGlobals();

    xmlMutexLock(xmlThrDefMutex);

#if defined(LIBXML_DOCB_ENABLED) && defined(LIBXML_LEGACY_ENABLED) && defined(LIBXML_SAX1_ENABLED)
    initdocbDefaultSAXHandler(&gs->docbDefaultSAXHandler);
#endif
#if defined(LIBXML_HTML_ENABLED) && defined(LIBXML_LEGACY_ENABLED)
    inithtmlDefaultSAXHandler(&gs->htmlDefaultSAXHandler);
#endif

    gs->oldXMLWDcompatibility = 0;
    gs->xmlBufferAllocScheme = xmlBufferAllocSchemeThrDef;
    gs->xmlDefaultBufferSize = xmlDefaultBufferSizeThrDef;
#if defined(LIBXML_SAX1_ENABLED) && defined(LIBXML_LEGACY_ENABLED)
    initxmlDefaultSAXHandler(&gs->xmlDefaultSAXHandler, 1);
#endif  
    gs->xmlDefaultSAXLocator.getPublicId = xmlSAX2GetPublicId;
    gs->xmlDefaultSAXLocator.getSystemId = xmlSAX2GetSystemId;
    gs->xmlDefaultSAXLocator.getLineNumber = xmlSAX2GetLineNumber;
    gs->xmlDefaultSAXLocator.getColumnNumber = xmlSAX2GetColumnNumber;
    gs->xmlDoValidityCheckingDefaultValue = 
         xmlDoValidityCheckingDefaultValueThrDef;
#if defined(DEBUG_MEMORY_LOCATION) | defined(DEBUG_MEMORY)
    gs->xmlFree = (xmlFreeFunc) xmlMemFree;
    gs->xmlMalloc = (xmlMallocFunc) xmlMemMalloc;
    gs->xmlMallocAtomic = (xmlMallocFunc) xmlMemMalloc;
    gs->xmlRealloc = (xmlReallocFunc) xmlMemRealloc;
    gs->xmlMemStrdup = (xmlStrdupFunc) xmlMemoryStrdup;
#else
    gs->xmlFree = (xmlFreeFunc) free;
    gs->xmlMalloc = (xmlMallocFunc) malloc;
    gs->xmlMallocAtomic = (xmlMallocFunc) malloc;
    gs->xmlRealloc = (xmlReallocFunc) realloc;
    gs->xmlMemStrdup = (xmlStrdupFunc) xmlStrdup;
#endif
    gs->xmlGetWarningsDefaultValue = xmlGetWarningsDefaultValueThrDef;
    gs->xmlIndentTreeOutput = xmlIndentTreeOutputThrDef;
    gs->xmlTreeIndentString = xmlTreeIndentStringThrDef;
    gs->xmlKeepBlanksDefaultValue = xmlKeepBlanksDefaultValueThrDef;
    gs->xmlLineNumbersDefaultValue = xmlLineNumbersDefaultValueThrDef;
    gs->xmlLoadExtDtdDefaultValue = xmlLoadExtDtdDefaultValueThrDef;
    gs->xmlParserDebugEntities = xmlParserDebugEntitiesThrDef;
    gs->xmlParserVersion = LIBXML_VERSION_STRING;
    gs->xmlPedanticParserDefaultValue = xmlPedanticParserDefaultValueThrDef;
    gs->xmlSaveNoEmptyTags = xmlSaveNoEmptyTagsThrDef;
    gs->xmlSubstituteEntitiesDefaultValue = 
        xmlSubstituteEntitiesDefaultValueThrDef;

    gs->xmlGenericError = xmlGenericErrorThrDef;
    gs->xmlStructuredError = xmlStructuredErrorThrDef;
    gs->xmlGenericErrorContext = xmlGenericErrorContextThrDef;
    gs->xmlStructuredErrorContext = xmlStructuredErrorContextThrDef;
    gs->xmlRegisterNodeDefaultValue = xmlRegisterNodeDefaultValueThrDef;
    gs->xmlDeregisterNodeDefaultValue = xmlDeregisterNodeDefaultValueThrDef;

	gs->xmlParserInputBufferCreateFilenameValue = xmlParserInputBufferCreateFilenameValueThrDef;
	gs->xmlOutputBufferCreateFilenameValue = xmlOutputBufferCreateFilenameValueThrDef;
    memset(&gs->xmlLastError, 0, sizeof(xmlError));

    xmlMutexUnlock(xmlThrDefMutex);
}
