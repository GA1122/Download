xsltExtElementLookup(xsltTransformContextPtr ctxt,
                     const xmlChar * name, const xmlChar * URI)
{
    xsltTransformFunction ret;

    if ((name == NULL) || (URI == NULL))
        return (NULL);

    if ((ctxt != NULL) && (ctxt->extElements != NULL)) {
        XML_CAST_FPTR(ret) = xmlHashLookup2(ctxt->extElements, name, URI);
        if (ret != NULL) {
            return(ret);
        }
    }

    ret = xsltExtModuleElementLookup(name, URI);

    return (ret);
}
