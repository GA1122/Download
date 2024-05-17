xsltUnregisterExtModuleFunction(const xmlChar * name, const xmlChar * URI)
{
    int ret;

    if ((xsltFunctionsHash == NULL) || (name == NULL) || (URI == NULL))
        return (-1);

    xmlMutexLock(xsltExtMutex);

    ret = xmlHashRemoveEntry2(xsltFunctionsHash, name, URI, NULL);

    xmlMutexUnlock(xsltExtMutex);

    return(ret);
}
