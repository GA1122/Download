xsltExtElementTest(xsltTransformContextPtr ctxt, xmlNodePtr node,
                   xmlNodePtr inst,
                   xsltElemPreCompPtr comp ATTRIBUTE_UNUSED)
{
    xmlNodePtr commentNode;

    if (testData == NULL) {
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltExtElementTest: not initialized,"
                         " calling xsltGetExtData\n");
        xsltGetExtData(ctxt, (const xmlChar *) XSLT_DEFAULT_URL);
        if (testData == NULL) {
            xsltTransformError(ctxt, NULL, inst,
                               "xsltExtElementTest: not initialized\n");
            return;
        }
    }
    if (ctxt == NULL) {
        xsltTransformError(ctxt, NULL, inst,
                           "xsltExtElementTest: no transformation context\n");
        return;
    }
    if (node == NULL) {
        xsltTransformError(ctxt, NULL, inst,
                           "xsltExtElementTest: no current node\n");
        return;
    }
    if (inst == NULL) {
        xsltTransformError(ctxt, NULL, inst,
                           "xsltExtElementTest: no instruction\n");
        return;
    }
    if (ctxt->insert == NULL) {
        xsltTransformError(ctxt, NULL, inst,
                           "xsltExtElementTest: no insertion point\n");
        return;
    }
    commentNode = xmlNewComment((const xmlChar *)
                                "libxslt:test element test worked");
    xmlAddChild(ctxt->insert, commentNode);
}
