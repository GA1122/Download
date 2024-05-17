xsltRegisterExtModuleTopLevel(const xmlChar * name, const xmlChar * URI,
                              xsltTopLevelFunction function)
{
    if ((name == NULL) || (URI == NULL) || (function == NULL))
        return (-1);

    if (xsltTopLevelsHash == NULL)
        xsltTopLevelsHash = xmlHashCreate(10);
    if (xsltTopLevelsHash == NULL)
        return (-1);

    xmlMutexLock(xsltExtMutex);

    xmlHashUpdateEntry2(xsltTopLevelsHash, name, URI,
                        XML_CAST_FPTR(function), NULL);

    xmlMutexUnlock(xsltExtMutex);

    return (0);
}
