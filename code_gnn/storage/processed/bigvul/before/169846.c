xsltExtFunctionTest(xmlXPathParserContextPtr ctxt,
                    int nargs ATTRIBUTE_UNUSED)
{
    xsltTransformContextPtr tctxt;
    void *data = NULL;

    tctxt = xsltXPathGetTransformContext(ctxt);

    if (testData == NULL) {
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltExtFunctionTest: not initialized,"
                         " calling xsltGetExtData\n");
        data = xsltGetExtData(tctxt, (const xmlChar *) XSLT_DEFAULT_URL);
        if (data == NULL) {
            xsltTransformError(tctxt, NULL, NULL,
                               "xsltExtElementTest: not initialized\n");
            return;
        }
    }
    if (tctxt == NULL) {
        xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
                           "xsltExtFunctionTest: failed to get the transformation context\n");
        return;
    }
    if (data == NULL)
        data = xsltGetExtData(tctxt, (const xmlChar *) XSLT_DEFAULT_URL);
    if (data == NULL) {
        xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
                           "xsltExtFunctionTest: failed to get module data\n");
        return;
    }
    if (data != testData) {
        xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
                           "xsltExtFunctionTest: got wrong module data\n");
        return;
    }
#ifdef WITH_XSLT_DEBUG_FUNCTION
    xsltGenericDebug(xsltGenericDebugContext,
                     "libxslt:test() called with %d args\n", nargs);
#endif
}
