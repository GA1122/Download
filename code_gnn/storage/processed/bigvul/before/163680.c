xmlBufMergeBuffer(xmlBufPtr buf, xmlBufferPtr buffer) {
    int ret = 0;

    if ((buf == NULL) || (buf->error)) {
	xmlBufferFree(buffer);
        return(-1);
    }
    CHECK_COMPAT(buf)
    if ((buffer != NULL) && (buffer->content != NULL) &&
             (buffer->use > 0)) {
        ret = xmlBufAdd(buf, buffer->content, buffer->use);
    }
    xmlBufferFree(buffer);
    return(ret);
}
