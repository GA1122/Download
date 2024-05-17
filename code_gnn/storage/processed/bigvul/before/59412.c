xmlCleanupParser(void) {
    if (!xmlParserInitialized)
	return;

    xmlCleanupCharEncodingHandlers();
#ifdef LIBXML_CATALOG_ENABLED
    xmlCatalogCleanup();
#endif
    xmlDictCleanup();
    xmlCleanupInputCallbacks();
#ifdef LIBXML_OUTPUT_ENABLED
    xmlCleanupOutputCallbacks();
#endif
#ifdef LIBXML_SCHEMAS_ENABLED
    xmlSchemaCleanupTypes();
    xmlRelaxNGCleanupTypes();
#endif
    xmlResetLastError();
    xmlCleanupGlobals();
    xmlCleanupThreads();  
    xmlCleanupMemory();
    xmlParserInitialized = 0;
}