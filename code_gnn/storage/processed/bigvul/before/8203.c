static int32_t get_iounit(V9fsPDU *pdu, V9fsPath *path)
{
    struct statfs stbuf;
    int32_t iounit = 0;
    V9fsState *s = pdu->s;

     
    if (!v9fs_co_statfs(pdu, path, &stbuf)) {
        iounit = stbuf.f_bsize;
        iounit *= (s->msize - P9_IOHDRSZ)/stbuf.f_bsize;
    }
    if (!iounit) {
        iounit = s->msize - P9_IOHDRSZ;
    }
    return iounit;
}
