xsltExtModuleTopLevelLookup(const xmlChar * name, const xmlChar * URI)
{
    xsltTopLevelFunction ret;

    if ((xsltTopLevelsHash == NULL) || (name == NULL) || (URI == NULL))
        return (NULL);

    xmlMutexLock(xsltExtMutex);

    XML_CAST_FPTR(ret) = xmlHashLookup2(xsltTopLevelsHash, name, URI);

    xmlMutexUnlock(xsltExtMutex);

     
    if (NULL == ret) {
        if (!xsltExtModuleRegisterDynamic(URI)) {
            xmlMutexLock(xsltExtMutex);

            XML_CAST_FPTR(ret) = xmlHashLookup2(xsltTopLevelsHash, name, URI);

            xmlMutexUnlock(xsltExtMutex);
        }
    }

    return (ret);
}
