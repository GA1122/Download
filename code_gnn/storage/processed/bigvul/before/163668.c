xmlBufEnd(xmlBufPtr buf)
{
    if ((!buf) || (buf->error))
        return NULL;
    CHECK_COMPAT(buf)

    return(&buf->content[buf->use]);
}
