xmlBufGetInputBase(xmlBufPtr buf, xmlParserInputPtr input) {
    size_t base;

    if ((input == NULL) || (buf == NULL) || (buf->error))
        return(-1);
    CHECK_COMPAT(buf)
    base = input->base - buf->content;
     
    if (base > buf->size) {
        xmlBufOverflowError(buf, "Input reference outside of the buffer");
        base = 0;
    }
    return(base);
}
