static int mboxlist_create_partition(const char *mboxname,
                                     const char *part,
                                     char **out)
{
    mbentry_t *parent = NULL;

    if (!part) {
        int r = mboxlist_findparent(mboxname, &parent);
        if (!r) part = parent->partition;
    }

     
    if (!part && config_defpartition)
        part = config_defpartition;

     
    if (!part)
        part = partlist_local_select();

     
    if (!part || (strlen(part) > MAX_PARTITION_LEN))
        goto err;

    if (!config_partitiondir(part))
        goto err;

    *out = xstrdupnull(part);

    mboxlist_entry_free(&parent);
    return 0;

err:
    mboxlist_entry_free(&parent);
    return IMAP_PARTITION_UNKNOWN;
}