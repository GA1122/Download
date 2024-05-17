xsltRegisterExtModule(const xmlChar * URI,
                      xsltExtInitFunction initFunc,
                      xsltExtShutdownFunction shutdownFunc)
{
    return xsltRegisterExtModuleFull(URI, initFunc, shutdownFunc,
                                     NULL, NULL);
}
