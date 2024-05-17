xmlBufDetach(xmlBufPtr buf) {
    xmlChar *ret;

    if (buf == NULL)
        return(NULL);
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE)
        return(NULL);
    if (buf->buffer != NULL)
        return(NULL);
    if (buf->error)
        return(NULL);

    ret = buf->content;
    buf->content = NULL;
    buf->size = 0;
    buf->use = 0;
    buf->compat_use = 0;
    buf->compat_size = 0;

    return ret;
}
