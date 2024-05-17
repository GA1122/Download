xsltXPathGetTransformContext(xmlXPathParserContextPtr ctxt)
{
    if ((ctxt == NULL) || (ctxt->context == NULL))
        return (NULL);
    return (ctxt->context->extra);
}
