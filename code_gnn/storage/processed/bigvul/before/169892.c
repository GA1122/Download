xsltUnregisterAllExtModuleTopLevel(void)
{
    xmlMutexLock(xsltExtMutex);

    xmlHashFree(xsltTopLevelsHash, NULL);
    xsltTopLevelsHash = NULL;

    xmlMutexUnlock(xsltExtMutex);
}
