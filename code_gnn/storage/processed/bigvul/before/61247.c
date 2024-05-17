static int mboxlist_changequota(const mbentry_t *mbentry, void *rock)
{
    int r = 0;
    struct mailbox *mailbox = NULL;
    const char *root = (const char *) rock;
    int res;
    quota_t quota_usage[QUOTA_NUMRESOURCES];

    assert(root);

    r = mailbox_open_iwl(mbentry->name, &mailbox);
    if (r) goto done;

    mailbox_get_usage(mailbox, quota_usage);

    if (mailbox->quotaroot) {
        quota_t quota_diff[QUOTA_NUMRESOURCES];

        if (strlen(mailbox->quotaroot) >= strlen(root)) {
             
            goto done;
        }

         
        for (res = 0; res < QUOTA_NUMRESOURCES ; res++) {
            quota_diff[res] = -quota_usage[res];
        }
        r = quota_update_useds(mailbox->quotaroot, quota_diff,
                               mailbox->name);
    }

     
    r = mailbox_set_quotaroot(mailbox, root);
    if (r) goto done;

     
    r = quota_update_useds(root, quota_usage, mailbox->name);

 done:
    mailbox_close(&mailbox);

    if (r) {
        syslog(LOG_ERR, "LOSTQUOTA: unable to change quota root for %s to %s: %s",
               mbentry->name, root, error_message(r));
    }

     
    return 0;
}
