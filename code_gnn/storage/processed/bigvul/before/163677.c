xmlBufIsEmpty(const xmlBufPtr buf)
{
    if ((!buf) || (buf->error))
        return(-1);
    CHECK_COMPAT(buf)

    return(buf->use == 0);
}
