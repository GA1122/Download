xsltInit (void) {
    if (initialized == 0) {
	initialized = 1;
#ifdef XSLT_LOCALE_WINAPI
	xsltLocaleMutex = xmlNewRMutex();
#endif
        xsltRegisterAllExtras();
    }
}
