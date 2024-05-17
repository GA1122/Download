xmlReadFile(const char *filename, const char *encoding, int options)
{
    xmlParserCtxtPtr ctxt;

    ctxt = xmlCreateURLParserCtxt(filename, options);
    if (ctxt == NULL)
        return (NULL);
    return (xmlDoRead(ctxt, NULL, encoding, options, 0));
}
