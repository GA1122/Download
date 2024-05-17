xmlBufSetInputBaseCur(xmlBufPtr buf, xmlParserInputPtr input,
                      size_t base, size_t cur) {
    if ((input == NULL) || (buf == NULL) || (buf->error))
        return(-1);
    CHECK_COMPAT(buf)
    input->base = &buf->content[base];
    input->cur = input->base + cur;
    input->end = &buf->content[buf->use];
    return(0);
}
