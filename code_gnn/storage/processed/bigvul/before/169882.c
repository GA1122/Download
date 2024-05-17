xsltRegisterExtPrefix(xsltStylesheetPtr style,
                      const xmlChar * prefix, const xmlChar * URI)
{
    xsltExtDefPtr def, ret;

    if ((style == NULL) || (URI == NULL))
        return (-1);

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    xsltGenericDebug(xsltGenericDebugContext,
	"Registering extension namespace '%s'.\n", URI);
#endif
    def = (xsltExtDefPtr) style->nsDefs;
#ifdef XSLT_REFACTORED
     
    while (def != NULL) {
        if (xmlStrEqual(URI, def->URI))
            return (1);
        def = def->next;
    }
#else
    while (def != NULL) {
        if (xmlStrEqual(prefix, def->prefix))
            return (-1);
        def = def->next;
    }
#endif
    ret = xsltNewExtDef(prefix, URI);
    if (ret == NULL)
        return (-1);
    ret->next = (xsltExtDefPtr) style->nsDefs;
    style->nsDefs = ret;

     
#ifdef XSLT_REFACTORED
     
#else
    if (xsltExtensionsHash != NULL) {
        xsltExtModulePtr module;

        xmlMutexLock(xsltExtMutex);
        module = xmlHashLookup(xsltExtensionsHash, URI);
        xmlMutexUnlock(xsltExtMutex);
        if (NULL == module) {
            if (!xsltExtModuleRegisterDynamic(URI)) {
                xmlMutexLock(xsltExtMutex);
                module = xmlHashLookup(xsltExtensionsHash, URI);
                xmlMutexUnlock(xsltExtMutex);
            }
        }
        if (module != NULL) {
            xsltStyleGetExtData(style, URI);
        }
    }
#endif
    return (0);
}
