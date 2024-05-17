xsltNewExtData(xsltExtModulePtr extModule, void *extData)
{
    xsltExtDataPtr cur;

    if (extModule == NULL)
        return (NULL);
    cur = (xsltExtDataPtr) xmlMalloc(sizeof(xsltExtData));
    if (cur == NULL) {
        xsltTransformError(NULL, NULL, NULL,
                           "xsltNewExtData : malloc failed\n");
        return (NULL);
    }
    cur->extModule = extModule;
    cur->extData = extData;
    return (cur);
}
