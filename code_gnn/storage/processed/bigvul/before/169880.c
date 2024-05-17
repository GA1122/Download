xsltRegisterExtModuleFunction(const xmlChar * name, const xmlChar * URI,
                              xmlXPathFunction function)
{
    if ((name == NULL) || (URI == NULL) || (function == NULL))
        return (-1);

    if (xsltFunctionsHash == NULL)
        xsltFunctionsHash = xmlHashCreate(10);
    if (xsltFunctionsHash == NULL)
        return (-1);

    xmlMutexLock(xsltExtMutex);

    xmlHashUpdateEntry2(xsltFunctionsHash, name, URI,
                        XML_CAST_FPTR(function), NULL);

    xmlMutexUnlock(xsltExtMutex);

    return (0);
}
