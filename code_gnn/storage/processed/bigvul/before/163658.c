xmlBufAvail(const xmlBufPtr buf)
{
    if ((!buf) || (buf->error))
        return 0;
    CHECK_COMPAT(buf)

    return(buf->size - buf->use);
}