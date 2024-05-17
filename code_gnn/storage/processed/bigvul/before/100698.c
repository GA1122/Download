xmlInitParser(void) {
    if (xmlParserInitialized != 0)
	return;

#ifdef LIBXML_THREAD_ENABLED
    __xmlGlobalInitMutexLock();
    if (xmlParserInitialized == 0) {
#endif
	xmlInitGlobals();
	xmlInitThreads();
	if ((xmlGenericError == xmlGenericErrorDefaultFunc) ||
	    (xmlGenericError == NULL))
	    initGenericErrorDefaultFunc(NULL);
	xmlInitMemory();
	xmlInitCharEncodingHandlers();
	xmlDefaultSAXHandlerInit();
	xmlRegisterDefaultInputCallbacks();
#ifdef LIBXML_OUTPUT_ENABLED
	xmlRegisterDefaultOutputCallbacks();
#endif  
#ifdef LIBXML_HTML_ENABLED
	htmlInitAutoClose();
	htmlDefaultSAXHandlerInit();
#endif
#ifdef LIBXML_XPATH_ENABLED
	xmlXPathInit();
#endif
	xmlParserInitialized = 1;
#ifdef LIBXML_THREAD_ENABLED
    }
    __xmlGlobalInitMutexUnlock();
#endif
}
