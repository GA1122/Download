xmlBufContent(const xmlBuf *buf)
{
    if ((!buf) || (buf->error))
        return NULL;

    return(buf->content);
}
