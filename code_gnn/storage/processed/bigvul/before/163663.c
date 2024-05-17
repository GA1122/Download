xmlBufCreate(void) {
    xmlBufPtr ret;

    ret = (xmlBufPtr) xmlMalloc(sizeof(xmlBuf));
    if (ret == NULL) {
	xmlBufMemoryError(NULL, "creating buffer");
        return(NULL);
    }
    ret->compat_use = 0;
    ret->use = 0;
    ret->error = 0;
    ret->buffer = NULL;
    ret->size = xmlDefaultBufferSize;
    ret->compat_size = xmlDefaultBufferSize;
    ret->alloc = xmlBufferAllocScheme;
    ret->content = (xmlChar *) xmlMallocAtomic(ret->size * sizeof(xmlChar));
    if (ret->content == NULL) {
	xmlBufMemoryError(ret, "creating buffer");
	xmlFree(ret);
        return(NULL);
    }
    ret->content[0] = 0;
    ret->contentIO = NULL;
    return(ret);
}
