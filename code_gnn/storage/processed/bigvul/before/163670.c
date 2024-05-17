xmlBufFree(xmlBufPtr buf) {
    if (buf == NULL) {
#ifdef DEBUG_BUFFER
        xmlGenericError(xmlGenericErrorContext,
		"xmlBufFree: buf == NULL\n");
#endif
	return;
    }

    if ((buf->alloc == XML_BUFFER_ALLOC_IO) &&
        (buf->contentIO != NULL)) {
        xmlFree(buf->contentIO);
    } else if ((buf->content != NULL) &&
        (buf->alloc != XML_BUFFER_ALLOC_IMMUTABLE)) {
        xmlFree(buf->content);
    }
    xmlFree(buf);
}
