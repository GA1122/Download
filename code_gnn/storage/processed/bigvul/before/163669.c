xmlBufErase(xmlBufPtr buf, size_t len) {
    if ((buf == NULL) || (buf->error))
        return(-1);
    CHECK_COMPAT(buf)
    if (len > buf->use)
        return(-1);
    buf->use -= len;
    buf->content[buf->use] = 0;
    UPDATE_COMPAT(buf)
    return(0);
}
