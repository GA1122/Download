xmlBufFromBuffer(xmlBufferPtr buffer) {
    xmlBufPtr ret;

    if (buffer == NULL)
        return(NULL);

    ret = (xmlBufPtr) xmlMalloc(sizeof(xmlBuf));
    if (ret == NULL) {
	xmlBufMemoryError(NULL, "creating buffer");
        return(NULL);
    }
    ret->use = buffer->use;
    ret->size = buffer->size;
    ret->compat_use = buffer->use;
    ret->compat_size = buffer->size;
    ret->error = 0;
    ret->buffer = buffer;
    ret->alloc = buffer->alloc;
    ret->content = buffer->content;
    ret->contentIO = buffer->contentIO;

    return(ret);
}
