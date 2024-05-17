xmlBufWriteCHAR(xmlBufPtr buf, const xmlChar *string) {
    if ((buf == NULL) || (buf->error))
        return(-1);
    CHECK_COMPAT(buf)
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE)
        return(-1);
    return(xmlBufCat(buf, string));
}
