xmlBufDump(FILE *file, xmlBufPtr buf) {
    size_t ret;

    if ((buf == NULL) || (buf->error != 0)) {
#ifdef DEBUG_BUFFER
        xmlGenericError(xmlGenericErrorContext,
		"xmlBufDump: buf == NULL or in error\n");
#endif
	return(0);
    }
    if (buf->content == NULL) {
#ifdef DEBUG_BUFFER
        xmlGenericError(xmlGenericErrorContext,
		"xmlBufDump: buf->content == NULL\n");
#endif
	return(0);
    }
    CHECK_COMPAT(buf)
    if (file == NULL)
	file = stdout;
    ret = fwrite(buf->content, sizeof(xmlChar), buf->use, file);
    return(ret);
}
