xmlBufCat(xmlBufPtr buf, const xmlChar *str) {
    if ((buf == NULL) || (buf->error))
        return(-1);
    CHECK_COMPAT(buf)
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) return -1;
    if (str == NULL) return -1;
    return xmlBufAdd(buf, str, -1);
}
