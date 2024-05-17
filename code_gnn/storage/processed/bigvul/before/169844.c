xsltExtElementPreCompTest(xsltStylesheetPtr style, xmlNodePtr inst,
                          xsltTransformFunction function)
{
    xsltElemPreCompPtr ret;

    if (style == NULL) {
        xsltTransformError(NULL, NULL, inst,
                           "xsltExtElementTest: no transformation context\n");
        return (NULL);
    }
    if (testStyleData == NULL) {
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltExtElementPreCompTest: not initialized,"
                         " calling xsltStyleGetExtData\n");
        xsltStyleGetExtData(style, (const xmlChar *) XSLT_DEFAULT_URL);
        if (testStyleData == NULL) {
            xsltTransformError(NULL, style, inst,
                               "xsltExtElementPreCompTest: not initialized\n");
            if (style != NULL)
                style->errors++;
            return (NULL);
        }
    }
    if (inst == NULL) {
        xsltTransformError(NULL, style, inst,
                           "xsltExtElementPreCompTest: no instruction\n");
        if (style != NULL)
            style->errors++;
        return (NULL);
    }
    ret = xsltNewElemPreComp(style, inst, function);
    return (ret);
}
