static int allmbox_p(void *rock,
                     const char *key,
                     size_t keylen,
                     const char *data,
                     size_t datalen)
{
    struct allmb_rock *mbrock = (struct allmb_rock *)rock;
    int r;

     
    if (keylen && key[0] == '$') return 0;

     
    mboxlist_entry_free(&mbrock->mbentry);

    r = mboxlist_parse_entry(&mbrock->mbentry, key, keylen, data, datalen);
    if (r) return 0;

    if (!(mbrock->flags & MBOXTREE_TOMBSTONES) && (mbrock->mbentry->mbtype & MBTYPE_DELETED))
        return 0;

    return 1;  
}
