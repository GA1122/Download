xsltShutdownExt(xsltExtDataPtr data, xsltStylesheetPtr style,
                const xmlChar * URI)
{
    xsltExtModulePtr module;

    if ((data == NULL) || (style == NULL) || (URI == NULL))
        return;
    module = data->extModule;
    if ((module == NULL) || (module->styleShutdownFunc == NULL))
        return;

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    xsltGenericDebug(xsltGenericDebugContext,
                     "Shutting down module : %s\n", URI);
#endif
    module->styleShutdownFunc(style, URI, data->extData);
     
}
