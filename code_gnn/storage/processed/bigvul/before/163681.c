xmlBufOverflowError(xmlBufPtr buf, const char *extra)
{
    __xmlSimpleError(XML_FROM_BUFFER, XML_BUF_OVERFLOW, NULL, NULL, extra);
    if ((buf) && (buf->error == 0))
        buf->error = XML_BUF_OVERFLOW;
}
