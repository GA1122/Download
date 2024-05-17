xmlBufBackToBuffer(xmlBufPtr buf) {
    xmlBufferPtr ret;

    if ((buf == NULL) || (buf->error))
        return(NULL);
    CHECK_COMPAT(buf)
    if (buf->buffer == NULL) {
        xmlBufFree(buf);
        return(NULL);
    }

    ret = buf->buffer;
     
    if (buf->use > INT_MAX) {
         
        xmlBufOverflowError(buf, "Used size too big for xmlBuffer");
        ret->use = INT_MAX;
        ret->size = INT_MAX;
    } else if (buf->size > INT_MAX) {
         
        xmlBufOverflowError(buf, "Allocated size too big for xmlBuffer");
        ret->size = INT_MAX;
    }
    ret->use = (int) buf->use;
    ret->size = (int) buf->size;
    ret->alloc = buf->alloc;
    ret->content = buf->content;
    ret->contentIO = buf->contentIO;
    xmlFree(buf);
    return(ret);
}
