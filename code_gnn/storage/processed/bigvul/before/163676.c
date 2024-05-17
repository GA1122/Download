xmlBufInflate(xmlBufPtr buf, size_t len) {
    if (buf == NULL) return(-1);
    xmlBufGrowInternal(buf, len + buf->size);
    if (buf->error)
        return(-1);
    return(0);
}
