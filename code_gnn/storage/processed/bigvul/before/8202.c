static V9fsFidState *get_fid(V9fsPDU *pdu, int32_t fid)
{
    int err;
    V9fsFidState *f;
    V9fsState *s = pdu->s;

    for (f = s->fid_list; f; f = f->next) {
        BUG_ON(f->clunked);
        if (f->fid == fid) {
             
            f->ref++;
             
            err = v9fs_reopen_fid(pdu, f);
            if (err < 0) {
                f->ref--;
                return NULL;
            }
             
            f->flags |= FID_REFERENCED;
            return f;
        }
    }
    return NULL;
}
