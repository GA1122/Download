xmlBufAddLen(xmlBufPtr buf, size_t len) {
    if ((buf == NULL) || (buf->error))
        return(-1);
    CHECK_COMPAT(buf)
    if (len > (buf->size - buf->use))
        return(-1);
    buf->use += len;
    UPDATE_COMPAT(buf)
    if (buf->size > buf->use)
        buf->content[buf->use] = 0;
    else
        return(-1);
    return(0);
}
