xmlBufMemoryError(xmlBufPtr buf, const char *extra)
{
    __xmlSimpleError(XML_FROM_BUFFER, XML_ERR_NO_MEMORY, NULL, NULL, extra);
    if ((buf) && (buf->error == 0))
        buf->error = XML_ERR_NO_MEMORY;
}
