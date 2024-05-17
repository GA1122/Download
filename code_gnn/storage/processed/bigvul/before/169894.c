xsltUnregisterExtModule(const xmlChar * URI)
{
    int ret;

    if (URI == NULL)
        return (-1);
    if (xsltExtensionsHash == NULL)
        return (-1);

    xmlMutexLock(xsltExtMutex);

    ret = xmlHashRemoveEntry(xsltExtensionsHash, URI,
                             (xmlHashDeallocator) xsltFreeExtModule);

    xmlMutexUnlock(xsltExtMutex);

    return (ret);
}
