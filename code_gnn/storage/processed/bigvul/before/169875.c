xsltNewExtModule(xsltExtInitFunction initFunc,
                 xsltExtShutdownFunction shutdownFunc,
                 xsltStyleExtInitFunction styleInitFunc,
                 xsltStyleExtShutdownFunction styleShutdownFunc)
{
    xsltExtModulePtr cur;

    cur = (xsltExtModulePtr) xmlMalloc(sizeof(xsltExtModule));
    if (cur == NULL) {
        xsltTransformError(NULL, NULL, NULL,
                           "xsltNewExtModule : malloc failed\n");
        return (NULL);
    }
    cur->initFunc = initFunc;
    cur->shutdownFunc = shutdownFunc;
    cur->styleInitFunc = styleInitFunc;
    cur->styleShutdownFunc = styleShutdownFunc;
    return (cur);
}
