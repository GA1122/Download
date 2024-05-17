xsltExtInitTest(xsltTransformContextPtr ctxt, const xmlChar * URI)
{
    if (testStyleData == NULL) {
        xsltGenericDebug(xsltGenericErrorContext,
                         "xsltExtInitTest: not initialized,"
                         " calling xsltStyleGetExtData\n");
        testStyleData = xsltStyleGetExtData(ctxt->style, URI);
        if (testStyleData == NULL) {
            xsltTransformError(ctxt, NULL, NULL,
                               "xsltExtInitTest: not initialized\n");
            return (NULL);
        }
    }
    if (testData != NULL) {
        xsltTransformError(ctxt, NULL, NULL,
                           "xsltExtInitTest: already initialized\n");
        return (NULL);
    }
    testData = (void *) "test data";
    xsltGenericDebug(xsltGenericDebugContext,
                     "Registered test module : %s\n", URI);
    return (testData);
}
