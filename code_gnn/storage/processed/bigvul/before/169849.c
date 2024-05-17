xsltExtModuleElementPreComputeLookup(const xmlChar * name,
                                     const xmlChar * URI)
{
    xsltExtElementPtr ext;

    if ((xsltElementsHash == NULL) || (name == NULL) || (URI == NULL))
        return (NULL);

    xmlMutexLock(xsltExtMutex);

    ext = (xsltExtElementPtr) xmlHashLookup2(xsltElementsHash, name, URI);

    xmlMutexUnlock(xsltExtMutex);

    if (ext == NULL) {
        if (!xsltExtModuleRegisterDynamic(URI)) {
            xmlMutexLock(xsltExtMutex);

            ext = (xsltExtElementPtr)
	          xmlHashLookup2(xsltElementsHash, name, URI);

            xmlMutexUnlock(xsltExtMutex);
        }
    }

    if (ext == NULL)
        return (NULL);
    return (ext->precomp);
}
