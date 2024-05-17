xsltUnregisterExtModuleTopLevel(const xmlChar * name, const xmlChar * URI)
{
    int ret;

    if ((xsltTopLevelsHash == NULL) || (name == NULL) || (URI == NULL))
        return (-1);

    xmlMutexLock(xsltExtMutex);

    ret = xmlHashRemoveEntry2(xsltTopLevelsHash, name, URI, NULL);

    xmlMutexUnlock(xsltExtMutex);

    return(ret);
}
