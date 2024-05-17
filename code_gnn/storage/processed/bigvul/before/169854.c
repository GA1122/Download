xsltExtShutdownTest(xsltTransformContextPtr ctxt,
                    const xmlChar * URI, void *data)
{
    if (testData == NULL) {
        xsltTransformError(ctxt, NULL, NULL,
                           "xsltExtShutdownTest: not initialized\n");
        return;
    }
    if (data != testData) {
        xsltTransformError(ctxt, NULL, NULL,
                           "xsltExtShutdownTest: wrong data\n");
    }
    testData = NULL;
    xsltGenericDebug(xsltGenericDebugContext,
                     "Unregistered test module : %s\n", URI);
}
