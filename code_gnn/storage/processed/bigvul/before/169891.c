xsltUnregisterAllExtModuleFunction(void)
{
    xmlMutexLock(xsltExtMutex);

    xmlHashFree(xsltFunctionsHash, NULL);
    xsltFunctionsHash = NULL;

    xmlMutexUnlock(xsltExtMutex);
}
