xsltUnregisterAllExtModules(void)
{
    if (xsltExtensionsHash == NULL)
        return;

    xmlMutexLock(xsltExtMutex);

    xmlHashFree(xsltExtensionsHash,
                (xmlHashDeallocator) xsltFreeExtModule);
    xsltExtensionsHash = NULL;

    xmlMutexUnlock(xsltExtMutex);
}
