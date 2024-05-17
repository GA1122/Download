xsltExtModuleFunctionLookup(const xmlChar * name, const xmlChar * URI)
{
    xmlXPathFunction ret;

    if ((xsltFunctionsHash == NULL) || (name == NULL) || (URI == NULL))
        return (NULL);

    xmlMutexLock(xsltExtMutex);

    XML_CAST_FPTR(ret) = xmlHashLookup2(xsltFunctionsHash, name, URI);

    xmlMutexUnlock(xsltExtMutex);

     
    if (NULL == ret) {
        if (!xsltExtModuleRegisterDynamic(URI)) {
            xmlMutexLock(xsltExtMutex);

            XML_CAST_FPTR(ret) =
                xmlHashLookup2(xsltFunctionsHash, name, URI);

            xmlMutexUnlock(xsltExtMutex);
        }
    }

    return ret;
}
