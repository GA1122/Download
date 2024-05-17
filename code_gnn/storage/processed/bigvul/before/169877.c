xsltRegisterExtElement(xsltTransformContextPtr ctxt, const xmlChar * name,
                       const xmlChar * URI, xsltTransformFunction function)
{
    if ((ctxt == NULL) || (name == NULL) ||
        (URI == NULL) || (function == NULL))
        return (-1);
    if (ctxt->extElements == NULL)
        ctxt->extElements = xmlHashCreate(10);
    if (ctxt->extElements == NULL)
        return (-1);
    return (xmlHashAddEntry2
            (ctxt->extElements, name, URI, XML_CAST_FPTR(function)));
}
