xsltGetExtData(xsltTransformContextPtr ctxt, const xmlChar * URI)
{
    xsltExtDataPtr data;

    if ((ctxt == NULL) || (URI == NULL))
        return (NULL);
    if (ctxt->extInfos == NULL) {
        ctxt->extInfos = xmlHashCreate(10);
        if (ctxt->extInfos == NULL)
            return (NULL);
        data = NULL;
    } else {
        data = (xsltExtDataPtr) xmlHashLookup(ctxt->extInfos, URI);
    }
    if (data == NULL) {
        void *extData;
        xsltExtModulePtr module;

        xmlMutexLock(xsltExtMutex);

        module = xmlHashLookup(xsltExtensionsHash, URI);

        xmlMutexUnlock(xsltExtMutex);

        if (module == NULL) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
            xsltGenericDebug(xsltGenericDebugContext,
                             "Not registered extension module: %s\n", URI);
#endif
            return (NULL);
        } else {
            if (module->initFunc == NULL)
                return (NULL);

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
            xsltGenericDebug(xsltGenericDebugContext,
                             "Initializing module: %s\n", URI);
#endif

            extData = module->initFunc(ctxt, URI);
            if (extData == NULL)
                return (NULL);

            data = xsltNewExtData(module, extData);
            if (data == NULL)
                return (NULL);
            if (xmlHashAddEntry(ctxt->extInfos, URI, (void *) data) < 0) {
                xsltTransformError(ctxt, NULL, NULL,
                                   "Failed to register module data: %s\n",
                                   URI);
                if (module->shutdownFunc)
                    module->shutdownFunc(ctxt, URI, extData);
                xsltFreeExtData(data);
                return (NULL);
            }
        }
    }
    return (data->extData);
}
