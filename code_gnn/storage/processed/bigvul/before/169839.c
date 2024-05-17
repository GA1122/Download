xsltCleanupGlobals(void)
{
    xsltUnregisterAllExtModules();
    xsltUnregisterAllExtModuleFunction();
    xsltUnregisterAllExtModuleElement();
    xsltUnregisterAllExtModuleTopLevel();

    xmlMutexLock(xsltExtMutex);
     
    if (NULL != xsltModuleHash) {
        xmlHashScan(xsltModuleHash, xsltHashScannerModuleFree, 0);
        xmlHashFree(xsltModuleHash, NULL);
        xsltModuleHash = NULL;
    }
    xmlMutexUnlock(xsltExtMutex);

    xmlFreeMutex(xsltExtMutex);
    xsltExtMutex = NULL;
    xsltFreeLocales();
    xsltUninit();
}
