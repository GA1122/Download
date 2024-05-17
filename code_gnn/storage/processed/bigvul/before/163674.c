xmlBufGrow(xmlBufPtr buf, int len) {
    size_t ret;

    if ((buf == NULL) || (len < 0)) return(-1);
    if (len == 0)
        return(0);
    ret = xmlBufGrowInternal(buf, len);
    if (buf->error != 0)
        return(-1);
    return((int) ret);
}
