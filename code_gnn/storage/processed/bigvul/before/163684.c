xmlBufSetAllocationScheme(xmlBufPtr buf,
                          xmlBufferAllocationScheme scheme) {
    if ((buf == NULL) || (buf->error != 0)) {
#ifdef DEBUG_BUFFER
        xmlGenericError(xmlGenericErrorContext,
		"xmlBufSetAllocationScheme: buf == NULL or in error\n");
#endif
        return(-1);
    }
    if ((buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) ||
        (buf->alloc == XML_BUFFER_ALLOC_IO))
        return(-1);
    if ((scheme == XML_BUFFER_ALLOC_DOUBLEIT) ||
        (scheme == XML_BUFFER_ALLOC_EXACT) ||
        (scheme == XML_BUFFER_ALLOC_HYBRID) ||
        (scheme == XML_BUFFER_ALLOC_IMMUTABLE) ||
	(scheme == XML_BUFFER_ALLOC_BOUNDED)) {
	buf->alloc = scheme;
        if (buf->buffer)
            buf->buffer->alloc = scheme;
        return(0);
    }
     
    if (scheme == XML_BUFFER_ALLOC_IO) {
        buf->alloc = XML_BUFFER_ALLOC_IO;
        buf->contentIO = buf->content;
    }
    return(-1);
}
