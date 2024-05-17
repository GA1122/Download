xsltInitCtxtExt(xsltExtDataPtr styleData, xsltInitExtCtxt * ctxt,
                const xmlChar * URI)
{
    xsltExtModulePtr module;
    xsltExtDataPtr ctxtData;
    void *extData;

    if ((styleData == NULL) || (ctxt == NULL) || (URI == NULL) ||
        (ctxt->ret == -1)) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltInitCtxtExt: NULL param or error\n");
#endif
        return;
    }
    module = styleData->extModule;
    if ((module == NULL) || (module->initFunc == NULL)) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltInitCtxtExt: no module or no initFunc\n");
#endif
        return;
    }

    ctxtData = (xsltExtDataPtr) xmlHashLookup(ctxt->ctxt->extInfos, URI);
    if (ctxtData != NULL) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltInitCtxtExt: already initialized\n");
#endif
        return;
    }

    extData = module->initFunc(ctxt->ctxt, URI);
    if (extData == NULL) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltInitCtxtExt: no extData\n");
#endif
    }
    ctxtData = xsltNewExtData(module, extData);
    if (ctxtData == NULL) {
        ctxt->ret = -1;
        return;
    }

    if (ctxt->ctxt->extInfos == NULL)
        ctxt->ctxt->extInfos = xmlHashCreate(10);
    if (ctxt->ctxt->extInfos == NULL) {
        ctxt->ret = -1;
        return;
    }

    if (xmlHashAddEntry(ctxt->ctxt->extInfos, URI, ctxtData) < 0) {
        xsltGenericError(xsltGenericErrorContext,
                         "Failed to register module data: %s\n", URI);
        if (module->shutdownFunc)
            module->shutdownFunc(ctxt->ctxt, URI, extData);
        xsltFreeExtData(ctxtData);
        ctxt->ret = -1;
        return;
    }
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    xsltGenericDebug(xsltGenericDebugContext, "Registered module %s\n",
                     URI);
#endif
    ctxt->ret++;
}
