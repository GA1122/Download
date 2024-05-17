EXPORTED mbentry_t *mboxlist_entry_create(void)
{
    mbentry_t *ret = xzmalloc(sizeof(mbentry_t));
     
    return ret;
}
